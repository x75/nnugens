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

#define SIMPLE_GET_BUF_1 \
	float fbufnum_1  = ZIN0(1); \
	fbufnum_1 = sc_max(0.f, fbufnum_1); \
	if (fbufnum_1 != unit->m_fbufnum_1) { \
		uint32 bufnum = (int)fbufnum_1; \
		World *world = unit->mWorld; \
		if (bufnum >= world->mNumSndBufs) { \
			int localBufNum = bufnum - world->mNumSndBufs; \
			Graph *parent = unit->mParent; \
			if(localBufNum <= parent->localBufNum) { \
				unit->m_buf_1 = parent->mLocalSndBufs + localBufNum; \
			} else { \
				bufnum = 0; \
				unit->m_buf_1 = world->mSndBufs + bufnum; \
			} \
		} else { \
			unit->m_buf_1 = world->mSndBufs + bufnum; \
		} \
		unit->m_fbufnum_1 = fbufnum_1; \
	} \
	SndBuf *buf = unit->m_buf_1; \



using namespace arma;

static InterfaceTable *ft;

////////////////////////////////////////////////////////////////////////////////
// Reservoir ugen: echo state network / liquid state machine
struct BufReservoir : public SCUnit
{
public:
  SndBuf *m_buf;
  float m_fbufnum;
  SndBuf *m_buf_1;
  float m_fbufnum_1;

  // reservoir size / model size
  int Wsize;
  // non-zero value locations
  umat Wloc;
  // reservoir recurrent update matrix
  SpMat W;
  // reservoir input matrix
  SpMat Wi;
  // mat A, B;
  // input vector
  vec x; // , y, y_, n, t1, t2;
  // state noise
  vec nu;
  // additional network parameters: time constant, spectral radius
  float tau, g;

  // // batch insertion of two values at (5, 6) and (9, 9)
  // umat locations;
  // locations << 5 << 9 << endr
  // << 6 << 9 << endr;

  // vec values;
  // values << 1.5 << 3.2 << endr;

  // sp_mat X(locations, values);

  BufReservoir() {
    m_fbufnum = std::numeric_limits<float>::quiet_NaN();
    BufReservoir * unit = this;
    SIMPLE_GET_BUF;
    SIMPLE_GET_BUF_1;

    Wsize = int(in(4));
    g = in(5);   // 0.99;
    tau = in(6); // 0.1;

    int ri, ci;
    // batch fill Mlocs
    Wloc = zeros<mat>(Wsize, 2);
    for (ri = 0; ri < m_buf_1->frames; ri++) {
      for (ci = 0; ci < 2; ci++) {
        // B(ri,ci) = table0[(ri*3)+ci] ; // buf->data[]
        
      }}

    // batch fill M

    // B = zeros<mat>(3,3);
    // store inputs
    x = zeros<vec>(2);
    // output
    // y  = randu<vec>(3) * 0.0;
    // noise
    // n = randu<vec>(3) * 0.01;
    // intermediate terms
    y_ = zeros<vec>(3);
    t1 = zeros<vec>(3);
    t2 = zeros<vec>(3);
    cout << "EvoPlast1: B = " << B << ", y = " << y << ", y_ = " << y_ << ", n = " << n << endl;

    set_calc_function<BufReservoir, &BufReservoir::next>();
    next(1);
  }

private:
  void next(int inNumSamples) {
    const SndBuf * buf = W_buf;
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


PluginLoad(ReservoirUGens) {
  ft = inTable;
  registerUnit<BufReservoir>(ft, "BufReservoir");
}
