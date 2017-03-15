#include "SC_PlugIn.hpp"
#include <armadillo>

using namespace arma;

static InterfaceTable *ft;

struct EvoPlast1 : public SCUnit {
public:
  EvoPlast1() {
    m_amp = 1.0; // in0(2);
    tau = 0.9; // in0(3);
    A = randu<mat>(2,2) * 3 - 1.5;
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
    B = mat("0, 0, 0; 0, 2.13513734,  3.73228563; 0, -2.00081304,  0.40366748");
    // B = mat("0, 0, 0; 0, 0.56363375, -0.19903545; 0, -0.3386425,   0.86326516");

    // B = randn<mat>(3,3) * 3;
    // // vec x(10);
    // // vec x = zeros<vec>(2);
    x = zeros<vec>(2);
    // y = zeros<vec>(3);
    y  = randu<vec>(3) * 0.0;
    y_ = zeros<vec>(3);
    n = randu<vec>(3) * 0.01;
    t1 = zeros<vec>(3);
    t2 = zeros<vec>(3);
    // colvec x = zeros
    // bla = 0;
    cout << "B = " << B << ", y = " << y << ", y_ = " << y_ << ", n = " << n << endl;

    set_calc_function<EvoPlast1, &EvoPlast1::next>();
    next(1);

  }
private:
  void next(int inNumSamples) {
    const float* in1 = in(0);
    const float* in2 = in(1);
    // const float* in_m_amp  = in(2);
    // const float* in_tau    = in(3);
    // const float* m_b   = in(2);
    // const float m_b_1_1   = in0(2);
    // const float m_b_1_2   = in0(3);
    // const float m_b_2_1   = in0(4);
    // const float m_b_2_2   = in0(5); 
    const float in_m_amp  = in0(2);
    const float in_tau    = in0(3);
   float* outbuf1 = out(0);
    float* outbuf2 = out(1);
    if (inNumSamples == 1) {
      cout << "next(1), setting B = " << B << ", m_amp + tau" << endl;
    }

    // m_amp = in_m_amp[0]; // in0(2);
    // tau   = in_tau[0];   // in0(3);
    m_amp = in_m_amp; // in0(2);
    tau   = in_tau;   // in0(3);
    // B(1,1) = m_b[0]; // m_b_1_1;
    // B(1,2) = m_b[1]; // m_b_1_2;
    // B(2,1) = m_b[2]; // m_b_2_1;
    // B(2,2) = m_b[3]; // m_b_2_2;

    if (inNumSamples == 1) {
    cout << "next(1), B = " << B << ", m_amp = " << m_amp << ", tau =" << tau << endl;
    }
    
    // cout << A*B.t() << endl;
    for (int i = 0; i < inNumSamples; i++) {
      x(0) = in1[i];
      x(1) = in2[i];
      n = randn<vec>(3) * 1e-3;
      y(0) = 0.0;
      // y = tanh((A*x) * 1.0 + (B*y) * 1.0) + n;
      t1 = (tau * y);
      t2 = ((1 - tau) * ((B * y) * m_amp));
      y_ = t1 + t2;
      y  = tanh(y_) + n;

      // cout << "t1 = " << t1 << ", t2 = " << t2 << ", y_ = " << y_ << ", y = " << y << endl;
    
      outbuf1[i] = y(1); // (y(0) + y(1)) * 0.5;
      outbuf2[i] = y(2);
    // outbuf[i] = (in1[i] + in2[i]) * 0.5 + bla;
  }
}
  mat A, B;
  vec x, y, y_, n;
  vec t1, t2;
  // int bla;
  float tau, m_amp;
};

PluginLoad(EvoPlast1UGens) {
  ft = inTable;
  registerUnit<EvoPlast1>(ft, "EvoPlast1");
}
