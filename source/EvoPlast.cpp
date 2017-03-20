#include "SC_PlugIn.hpp"
#include <armadillo>
#include <limits>

// TODO: matrix params via buffer
// TODO: cool defaults

// acks
// - sc-dev discussion linalg library / arma
// - tim blechmann NovaOsc unit, example SC_PlugIn.hpp based buffer ugen

// quick ref arma mat/vec examples

// A = randu<mat>(3,3) * 3 - 1.5;
// B = randn<mat>(2,2) * 0.3;
// B = randn<mat>(3,3);
// M = [[0.69656214,  0.33208246],
// [ 0.41712419,  0.56571223]]
// M = [[ 1.65149195, -2.1663804 ], [ 1.09012667,  0.47483937]]

// B = zeros<mat>(3,3);
// B(1,1) = 1.65149195;
// B(1,2) = -2.1663804;
// B(2,2) = 1.09012667;
// B(2,1) = 0.47483937;

// B = mat("0, 0, 0; 0, 0.35410052,  0.36185423; 0, 1.29750725,  0.29797154");
// B = mat("0, 0, 0; 0, 0.15237389, -7.50620661; 0., 0.47059135,  0.72873167");
// B = mat("0, 0, 0; 0, 0.48566475,  0.71121745;  0, 0.35990499,  0.53510189");
// B = mat("0, 0, 0; 0, 2.13513734,  3.73228563; 0, -2.00081304,  0.40366748");
// B = mat("0, 0, 0; 0, 0.56363375, -0.19903545; 0, -0.3386425,   0.86326516");

// B = randn<mat>(3,3) * 3;

// vec x(10);
// vec x = zeros<vec>(2);

// colvec x = zeros

using namespace arma;

static InterfaceTable *ft;


////////////////////////////////////////////////////////////////////////////////
// first version, basic recurrent network
struct EvoPlast1 : public SCUnit {
public:

  mat B;
  vec x, y, y_, n, t1, t2;
  float tau, m_amp;

  EvoPlast1() {
    m_amp = 1.0;
    tau = 0.9;

    B = zeros<mat>(3,3);
    // store inputs
    x = zeros<vec>(2);
    // output
    y  = randu<vec>(3) * 0.0;
    // noise
    n = randu<vec>(3) * 0.01;
    // intermediate terms
    y_ = zeros<vec>(3);
    t1 = zeros<vec>(3);
    t2 = zeros<vec>(3);
    cout << "EvoPlast1: B = " << B << ", y = " << y << ", y_ = " << y_ << ", n = " << n << endl;

    set_calc_function<EvoPlast1, &EvoPlast1::next>();
    next(1);
  }

private:
  void next(int inNumSamples) {
    const float* in1 = in(0);
    const float* in2 = in(1);
    const float in_m_amp  = in0(2);
    const float in_tau    = in0(3);
    float* outbuf1 = out(0);
    float* outbuf2 = out(1);

    // if (inNumSamples == 1) {
    //   cout << "next(1), setting B = " << B << ", m_amp + tau" << endl;
    // }

    m_amp = in_m_amp;
    tau   = in_tau;

    if (inNumSamples == 1) {
      cout << "next(1), B = " << B << ", m_amp = " << m_amp << ", tau =" << tau << endl;
    }
    
    for (int i = 0; i < inNumSamples; i++) {
      // x(0) = in1[i];
      // x(1) = in2[i];
      // sample noise
      n = randn<vec>(3) * 1e-3;
      // bias
      y(0) = 1.0;
      // intermediate
      t1 = (tau * y);
      t2 = ((1 - tau) * ((B * y) * m_amp));
      y_ = t1 + t2;
      // output
      y  = tanh(y_) + n;

      // cout << "t1 = " << t1 << ", t2 = " << t2 << ", y_ = " << y_ << ", y = " << y << endl;
    
      outbuf1[i] = y(1);
      outbuf2[i] = y(2);
  }
}
};

////////////////////////////////////////////////////////////////////////////////
// second version, recurrent network taking parameters from buffer
struct BufEvoPlast1 : public SCUnit
{
public:
  SndBuf *m_buf;
  float m_fbufnum;

  mat A, B;
  vec x, y, y_, n, t1, t2;
  float tau, m_amp;

  BufEvoPlast1() {
    m_fbufnum = std::numeric_limits<float>::quiet_NaN();
    BufEvoPlast1 * unit = this;
    SIMPLE_GET_BUF;

    m_amp = 1.0;
    tau = 0.9;

    B = zeros<mat>(3,3);
    // store inputs
    x = zeros<vec>(2);
    // output
    y  = randu<vec>(3) * 0.0;
    // noise
    n = randu<vec>(3) * 0.01;
    // intermediate terms
    y_ = zeros<vec>(3);
    t1 = zeros<vec>(3);
    t2 = zeros<vec>(3);
    cout << "EvoPlast1: B = " << B << ", y = " << y << ", y_ = " << y_ << ", n = " << n << endl;

    set_calc_function<BufEvoPlast1, &BufEvoPlast1::next>();
    next(1);
  }

private:
  void next(int inNumSamples) {
    const SndBuf * buf = m_buf;
    const float *table0 = buf->data;
    const int   bufsize = buf->samples;
    const int32 mask = buf->mask;

    // cout << "bufsize = " << bufsize << endl;

    const float* in1 = in(0);
    const float* in2 = in(1);
    const float in_m_amp  = in0(2);
    const float in_tau    = in0(3);
    float* outbuf1 = out(0);
    float* outbuf2 = out(1);

    // buf indices
    int ri, ci;
    // if (inNumSamples == 1) {
    //   cout << "next(1), setting B = " << B << ", m_amp + tau" << endl;
    //   for (int j = 0; j < bufsize; j++) {
    //     // cout << "buf[" << "blub" << "] = " << buf[j] << endl;
    //     cout << buf->data[j] << endl;
    //   }
    // }

    // copy matrix parameters from buffer
    for (ri = 0; ri < 3; ri++) {
      for (ci = 0; ci < 3; ci++) {
        B(ri,ci) = table0[(ri*3)+ci] ; // buf->data[]
      }}

    m_amp = in_m_amp;
    tau   = in_tau;

    if (inNumSamples == 1) {
      cout << "next(1), B = " << B << ", m_amp = " << m_amp << ", tau =" << tau << endl;
    }
    
    // cout << A*B.t() << endl;
    for (int i = 0; i < inNumSamples; i++) {
      // x(0) = in1[i];
      // x(1) = in2[i];
      // sample noise
      n = randn<vec>(3) * 1e-3;
      // set bias constant
      y(0) = 1.0;
      // couple input
      // y = tanh((A*x) * 1.0 + (B*y) * 1.0) + n;
      t1 = (tau * y);
      t2 = ((1 - tau) * ((B * y) * m_amp));
      y_ = t1 + t2;
      y  = tanh(y_) + n;

      // cout << "t1 = " << t1 << ", t2 = " << t2 << ", y_ = " << y_ << ", y = " << y << endl;
    
      outbuf1[i] = y(1);
      outbuf2[i] = y(2);
    }
  }
};

////////////////////////////////////////////////////////////////////////////////
// third version, recurrent double dynamics network aka ep3
struct BufEvoPlast2 : public SCUnit
{
public:
  SndBuf *m_buf;
  float m_fbufnum;

  // two networks: fast + slow
  int fn_s_dim, sn_s_dim, sn_i_dim;
  mat fn_M, fn_M_vecced, sn_M;
  float fn_tau, sn_tau, m_amp;
  vec fn_x, sn_x, fn_y, sn_y, fn_x_, sn_x_, sn_x__, fn_n, sn_n, fn_t1, sn_t1, fn_t2, sn_t2;

  BufEvoPlast2() {
    m_fbufnum = std::numeric_limits<float>::quiet_NaN();
    BufEvoPlast2 * unit = this;
    SIMPLE_GET_BUF;

    // fast state dimension
    fn_s_dim = 2 + 1; // two states + bias
    // fast time constant
    fn_tau = 0.5; // ep4
    // fast state
    fn_x = zeros<vec>(fn_s_dim);
    // fast matrix
    fn_M = zeros<mat>(fn_s_dim,fn_s_dim);
    fn_M_vecced = zeros<vec>(fn_s_dim * fn_s_dim);
    // fast output
    fn_y  = randu<vec>(fn_s_dim) * 0.0;
    // fast noise
    fn_n = randu<vec>(fn_s_dim) * 0.01;
    // fast intermediate terms
    fn_x_ = zeros<vec>(fn_s_dim);
    fn_t1 = zeros<vec>(fn_s_dim);
    fn_t2 = zeros<vec>(fn_s_dim);

    // slow time constant
    sn_tau = 0.8;
    // slow state dimension
    sn_s_dim = fn_s_dim * fn_s_dim; // size(fn_M); // 3x3 = 9
    sn_i_dim = sn_s_dim + fn_s_dim; // 3 + 9 = 12
    // slow matrix
    // sn_M = randu<mat>(sn_s_dim, sn_i_dim) - 0.5 * 0.02;
    sn_M = zeros<mat>(sn_s_dim, sn_i_dim);
    // slow output
    sn_x  = randu<vec>(sn_s_dim) * 0.0;
    // slow noise
    sn_n = randu<vec>(sn_s_dim) * 0.01;
    // slow intermediate terms
    sn_x_ = zeros<vec>(sn_s_dim);
    sn_t1 = zeros<vec>(sn_s_dim);
    sn_t2 = zeros<vec>(sn_s_dim);
    cout << "BufEvoPlast2: fast: fn_M = " << fn_M << ", fn_x = " << fn_x << ", fn_x_ = " << fn_x_ << ", fn_n = " << fn_n << endl;
    cout << "BufEvoPlast2: fast: sn_M = " << sn_M << ", sn_x = " << sn_x << ", sn_x_ = " << sn_x_ << ", sn_n = " << sn_n << endl;

    m_amp = 1.0;

    set_calc_function<BufEvoPlast2, &BufEvoPlast2::next>();
    next(1);
  }

private:
  void next(int inNumSamples) {
    const SndBuf * buf = m_buf;
    const float *table0 = buf->data;
    const int   bufsize = buf->samples;
    const int32 mask = buf->mask;

    // cout << "bufsize = " << bufsize << endl;

    // const float* in1 = in(0);
    // const float* in2 = in(1);
    const float in_m_amp  = in0(2);
    const float in_fn_tau    = in0(3);
    float* outbuf1 = out(0);
    float* outbuf2 = out(1);

    // buf indices
    int ri, ci;

    // copy fast matrix parameters from buffer
    // for (ri = 0; ri < fn_s_dim; ri++) {
    //   for (ci = 0; ci < fn_s_dim; ci++) {
    //     fn_M(ri,ci) = table0[(ri*fn_s_dim)+ci] ; // buf->data[]
    //   }}

    // copy slow matrix parameters from buffer
    for (ri = 0; ri < sn_s_dim; ri++) {
      for (ci = 0; ci < sn_i_dim; ci++) {
        sn_M(ri,ci) = table0[(ri*sn_s_dim) + ci] ; // buf->data[]
      }}

    m_amp = in_m_amp;
    fn_tau   = 0.5; // in_fn_tau;

    if (inNumSamples == 1) {
      cout << "fn_M = " << fn_M << ", sn_M = " << sn_M << endl;
      // cout << "next(1), setting B = " << B << ", m_amp + tau" << endl;
      // for (int j = 0; j < bufsize; j++) {
      //   // cout << "buf[" << "blub" << "] = " << buf[j] << endl;
      //   cout << buf->data[j] << endl;
      // }
    }

    // if (inNumSamples == 1) {
    //   cout << "next(1), B = " << B << ", m_amp = " << m_amp << ", tau =" << tau << endl;
    // }
    
    // main update equation
    for (int i = 0; i < inNumSamples; i++) {
      // x(0) = in1[i];
      // x(1) = in2[i];
      // fast network
      fn_t1 = fn_tau * fn_x;
      fn_t2 = (1 - fn_tau) * (fn_M * fn_x);
      fn_x_ = fn_t1 + fn_t2;
      fn_x_(0) = 10.0; // bias
      // sample noise
      fn_n = randn<vec>(fn_s_dim) * 1e-3;
      fn_x = tanh(fn_x_) + fn_n;

      // slow network
      fn_M_vecced = vectorise(fn_M, 0);
      sn_x__ = join_cols(fn_x, fn_M_vecced); // combined state / input
      // sn_t1 = sn_tau * (sn_M * fn_M_vecced);
      sn_t1 = 1.0 * fn_M_vecced;
      sn_t2 = (1 - sn_tau) * (sn_M * sn_x__);
      sn_x_ = sn_t1 + sn_t2;
      // hacky output mod
      sn_x = tanh(0.1 * sn_x_) * 10.0; // noise?
      // copy back to fn_M
      // fn_M_vecced = mat(sn_x);
      fn_M = reshape(sn_x, fn_s_dim, fn_s_dim);
      // cout << reshape(sn_x, fn_s_dim, fn_s_dim) << endl;
      // set bias constant
      // y(0) = 1.0;
      // couple input
      // y = tanh((A*x) * 1.0 + (B*y) * 1.0) + n;
      // t2 = ((1 - tau) * ((B * y) * m_amp));
      // y_ = t1 + t2;
      // y  = tanh(y_) + n;

      // cout << "t1 = " << t1 << ", t2 = " << t2 << ", y_ = " << y_ << ", y = " << y << endl;
    
      outbuf1[i] = fn_x(1);
      outbuf2[i] = fn_x(2);
    }
  }
};


PluginLoad(EvoPlast1UGens) {
  ft = inTable;
  registerUnit<EvoPlast1>(ft, "EvoPlast1");
  registerUnit<BufEvoPlast1>(ft, "BufEvoPlast1");
  registerUnit<BufEvoPlast2>(ft, "BufEvoPlast2");
}
