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

	// fbufnum = sc_max(0.f, fbufnum);                               \
	// if (fbufnum1 != unit->m_fbufnum1) {                           \
        // else { \
        //   cout << "in fbufnum1 weird" << endl; \
        // } \
	// if (fbufnum2 != unit->m_fbufnum2) { \
        // else { \
        //   cout << "in fbufnum1 weird" << endl; \
        // } \

// #define SIMPLE_GET_BUF_2 \
// 	float fbufnum1  = ZIN0(0); \
// 	float fbufnum2  = ZIN0(1); \
//         printf("MAC: fbufnum1 = %f, fbufnum2 = %f", fbufnum1, fbufnum2); \
//         //std::cout << "pre:  fbufnum1 = " << fbufnum1 << ", fbufnum2 = " << fbufnum2 << endl; \
//         //cout << "pre:  unit->m_fbufnum1 = " << unit->m_fbufnum1 << ", unit->m_fbufnum2 = " << unit->m_fbufnum2 << endl; \
// 	if (fbufnum1 < 0.f || fbufnum2 < 0.f) { ZOUT0(0) = -1.f; return; } \
//         World *world = unit->mWorld;                                    \
//         uint32 bufnum1 = (int)fbufnum1;                                   \
//         uint32 bufnum2 = (int)fbufnum2;                                   \
//         //std::cout << "in fbufnum1" << endl;                           \
// 		if (bufnum1 >= world->mNumSndBufs) { \
// 			int localBufNum = bufnum1 - world->mNumSndBufs; \
// 			Graph *parent = unit->mParent; \
// 			if(localBufNum <= parent->localBufNum) { \
// 				unit->m_buf1 = parent->mLocalSndBufs + localBufNum; \
// 			} else { \
// 				bufnum1 = 0; \
// 				unit->m_buf1 = world->mSndBufs + bufnum; \
// 			} \
// 		} else { \
// 			unit->m_buf1 = world->mSndBufs + bufnum1; \
// 		} \
// 		unit->m_fbufnum1 = fbufnum1; \
// 	} \
//          //std::cout << "in fbufnum2" << endl;       \
// 		if (bufnum2 >= world->mNumSndBufs) { \
// 			int localBufNum = bufnum2 - world->mNumSndBufs; \
// 			Graph *parent = unit->mParent; \
// 			if(localBufNum <= parent->localBufNum) { \
// 				unit->m_buf2 = parent->mLocalSndBufs + localBufNum; \
// 			} else { \
// 				bufnum2 = 0; \
// 				unit->m_buf2 = world->mSndBufs + bufnum; \
// 			} \
// 		} else { \
// 			unit->m_buf2 = world->mSndBufs + bufnum2; \
// 		} \
// 		unit->m_fbufnum2 = fbufnum2; \
// 	} \
//          // std::cout << "post: unit->m_fbufnum1 = " << unit->m_fbufnum1 << ", unit->m_fbufnum2 = " << unit->m_fbufnum2 << endl; \
// 	SndBuf *buf1 = unit->m_buf1;                                    \
// 	SndBuf *buf2 = unit->m_buf2;            \


// #define SIMPLE_GET_BUF_1 \
// 	float fbufnum_1  = ZIN0(0); \
// 	float fbufnum_2  = ZIN0(1); \
// 	if (fbufnum1 < 0.f || fbufnum2 < 0.f) { ZOUT0(0) = -1.f; return; } \
// 	// fbufnum_1 = sc_max(0.f, fbufnum_1);                           \
// 	uint32 ibufnum1 = (int)fbufnum1; \
// 	uint32 ibufnum2 = (int)fbufnum2; \
// 	World *world = unit->mWorld; \
// 	SndBuf *buf1; \
// 	SndBuf *buf2; \
//         if (fbufnum_1 != unit->m_fbufnum_1) {   \
// 		uint32 bufnum = (int)fbufnum_1; \
// 		World *world = unit->mWorld; \
// 		if (bufnum >= world->mNumSndBufs) { \
// 			int localBufNum = bufnum - world->mNumSndBufs; \
// 			Graph *parent = unit->mParent; \
// 			if(localBufNum <= parent->localBufNum) { \
// 				unit->m_buf_1 = parent->mLocalSndBufs + localBufNum; \
// 			} else { \
// 				bufnum = 0; \
// 				unit->m_buf_1 = world->mSndBufs + bufnum; \
// 			} \
// 		} else { \
// 			unit->m_buf_1 = world->mSndBufs + bufnum; \
// 		} \
// 		unit->m_fbufnum_1 = fbufnum_1; \
//         }                                                \
//         SndBuf *m_buf_1 = unit->m_buf_1;              \

using namespace arma;

static InterfaceTable *ft;

////////////////////////////////////////////////////////////////////////////////
// Reservoir ugen: echo state network / liquid state machine
struct BufReservoir : public SCUnit
{
public:
  SndBuf *m_buf;
  float m_fbufnum;
  // SndBuf *m_buf2;
  // float m_fbufnum2;

  // reservoir size / model size
  float wsize;
  int iwsize;
  float insize;
  int iinsize;
  int ioutsize;
  // non-zero value locations
  // umat Wloc;
  // vec Wval;
  // reservoir recurrent update matrix
  sp_mat W;
  mat W_;
  umat Wloc;
  vec Wval;
  // reservoir input matrix
  sp_mat Wi;
  umat Wiloc;
  vec Wival;
  // global readout feedback
  sp_mat Wf;

  
  // mat A, B;
  // input vector
  vec u;
  // state 
  vec x; // raw activation state
  vec x_; // raw activation state
  vec t1, t2, t3;
  vec r; // post activation function state
  // state noise
  vec nu;
  // additional network parameters: time constant, spectral radius
  // float *tau, *g;

  // // batch insertion of two values at (5, 6) and (9, 9)
  // umat Wloc;
  // Wloc << 5 << 9 << endr
  // << 6 << 9 << endr;

  // vec values;
  // values << 1.5 << 3.2 << endr;

  // sp_mat X(Wloc, values);

  int block_cnt;
  
  BufReservoir() {
    m_fbufnum = std::numeric_limits<float>::quiet_NaN();
    // m_fbufnum2 = std::numeric_limits<float>::quiet_NaN();
    BufReservoir * unit = this;
    // SIMPLE_GET_BUF;
    // SIMPLE_GET_BUF_1;
    // cout << "pre:  calling: m_fbufnum1 = " << m_fbufnum1 << ", m_fbufnum2 = " << m_fbufnum2 << endl;

    // SIMPLE_GET_BUF_2;

    // cout << "post: calling: m_fbufnum1 = " << m_fbufnum1 << ", m_fbufnum2 = " << m_fbufnum2 << endl;

    SIMPLE_GET_BUF;

    const float *insize = in(2);
    const float *wsize  = in(3);
    const float *g      = in(4);   // 0.99;
    const float *tau    = in(5); // 0.1;

    iwsize  = static_cast<int>(*wsize);
    iinsize = static_cast<int>(*insize);
    ioutsize = 1;
    // iwsize = 10;
    // iwsize = (int)(*wsize);
    block_cnt = 0;

    // cout << "iwsize = " << iwsize << endl;
    
    int ri, ci, nonzero_elem;
    // batch fill Mlocs
    // Wloc = zeros<umat>(m_buf2->frames, 2);
    // Wval = zeros<vec>(m_buf1->frames);
    // for (ri = 0; ri < m_buf2->frames; ri++) {
    // Wloc << 5 << 9 << endr
    // << 6 << 9 << endr;

    // values << 1.5 << 3.2 << endr;

    // cout << m_buf->data[0] << endl;

    // recurrent matrix
    nonzero_elem = 0;
    Wloc = zeros<umat>(0,2);
    Wval    = zeros<vec>(0,1);
    W_   = zeros<mat>(iwsize, iwsize);
    
    for (ri = 0; ri < iwsize; ri++) {
      for (ci = 0; ci < iwsize; ci++) {
        if (abs(m_buf->data[(ri * iwsize)+ci]) > 0.0) {
          // cout << "row " << ri << ", col " << ci << ", val = " << m_buf->data[(ri * iwsize)+ci] << endl;
          Wloc.insert_rows(nonzero_elem, 1);
          Wloc(nonzero_elem, 0) = ri;
          Wloc(nonzero_elem, 1) = ci;
          Wval.insert_rows(nonzero_elem, 1);
          Wval(nonzero_elem) = m_buf->data[(ri * iwsize)+ci];
          nonzero_elem++;
          W_(ri,ci) = m_buf->data[(ri * iwsize)+ci];
        }
    //   // B(ri,ci) = table0[(ri*3)+ci] ; // buf->data[]
    //   // Wloc(ri,0) = m_buf2->data[(ri*2)];
    //   // Wloc(ri,1) = m_buf2->data[(ri*2)+1];
    //   // Wval(ri)   = m_buf1->data[ri];
      }
    }
    // cout << "Wloc = " << Wloc << endl;
    // cout << "Wval = " << Wval << endl;
    // sparse batch initialization
    W = sp_mat(Wloc.t(), Wval, iwsize, iwsize);

    // W = W_;
    
    //  normalize_spectral_radius
    vec eigval;
    mat eigvec;
    float max_ev = 0;

    eigs_sym(eigval, eigvec, W, 10, "lm"); //iwsize);  // find 5 eigenvalues/eigenvectors
    max_ev = max(abs(eigval));
    // cout << "pre  eigvals = " << eigval << endl;
    cout << "pre  max_ev = " << max_ev << endl;
    sp_mat W_1 = W / max_ev;
    W = W_1 * (*g);
    eigs_sym(eigval, eigvec, W, 10, "lm"); //iwsize);  // find 5 eigenvalues/eigenvectors
    max_ev = max(abs(eigval));
    // cout << "post eigvals = " << eigval << endl;
    cout << "post max_ev = " << max_ev << endl;
    
    // input weights
    Wi = sprandu<sp_mat>(iwsize, iinsize, 0.5);

    // feedback weights
    Wf = sprandu<sp_mat>(iwsize, ioutsize, 0.2);

    // input
    u = zeros<vec>(iinsize);
    // state
    r  = randn<vec>(iwsize) * 0.1; // zeros<vec>(iwsize);
    x  = randn<vec>(iwsize) * 0.1; // zeros<vec>(iwsize);
    x_ = zeros<vec>(iwsize) * 0.1; // zeros<vec>(iwsize);
    nu = zeros<vec>(iwsize);
    t1 = zeros<vec>(iwsize); // zeros<vec>(iwsize);
    t2 = zeros<vec>(iwsize); // zeros<vec>(iwsize);
    t3 = zeros<vec>(iwsize); // zeros<vec>(iwsize);
    // output
    // y  = randu<vec>(3) * 0.0;
    // noise
    // n = randu<vec>(3) * 0.01;
    // intermediate terms
    // y_ = zeros<vec>(3);
    // t1 = zeros<vec>(3);
    // t2 = zeros<vec>(3);
    // cout << "EvoPlast1: B = " << B << ", y = " << y << ", y_ = " << y_ << ", n = " << n << endl;

    set_calc_function<BufReservoir, &BufReservoir::next>();
    next(1);
  }

private:
  void next(int inNumSamples) {
    const SndBuf * buf1 = m_buf; // W_buf;
    // const SndBuf * buf2 = m_buf2; // W_buf;
    const float *table0 = buf1->data;
    const int   bufsize = buf1->samples;
    const int32 mask = buf1->mask;

    // cout << "bufsize = " << bufsize << endl;

    // const float* in1 = in(0);
    const float* in1     = in(1);
    const float in_insize = in0(2);
    const float in_wsize  = in0(3);
    const float in_g     = in0(4);
    const float in_tau   = in0(5);
    float* outbuf1 = out(0);
    float* outbuf2 = out(1);

    // buf indices
    int ri, ci;
    // copy matrix parameters from buffer
    if (inNumSamples == 1) {
      cout << "next(1), bufsize = " << bufsize << endl;
      // for (ri = 0; ri < iwsize; ri++) {
      //   for (ci = 0; ci < iwsize; ci++) {
      //     // cout << "table val " << table0[(ri*iwsize)+ci] << endl;
      //     // printf("table val = %f", table0[(ri*iwsize)+ci]);
      //     // W(ri,ci) = table0[(ri*iwsize)+ci] ; // buf->data[]
      //   }
      // }
    }


    if (block_cnt > 0) {
    // cout << "W = " << W << ", x = " << x << ", r = " << r << ", nu = " << nu << endl;
    // cout << "in_g " << in_g << endl;
    // cout << "in_tau " << in_tau << endl;
      // cout << "|x| = " << norm(x, 2) << ", |x_| = " << norm(x, 2) << ", |r| = " << norm(r, 2) << ", |u| = " << norm(u, 2) << ", |t1| = " << norm(t1, 2) << ", |t2| = " << norm(t2, 2) << ", |t3| = " << norm(t3, 2) << endl;

    for (int i = 0; i < inNumSamples; i++) {
      u(0) = in1[i];
      // x(1) = in2[i];
      // x_ = x;
      t1 = (1 - in_tau) * x;
      t2 = W * r;
      t3 = Wi * u;
      x = t1 + (in_tau * t2);
      // x = x_.copy();
      nu = randn<vec>(iwsize) * 1e-3;
      r = tanh(x) + nu;
      // // sample noise
      // n = randn<vec>(3) * 1e-3;
      // // set bias constant
      // y(0) = 1.0;
      // // couple input
      // // y = tanh((A*x) * 1.0 + (B*y) * 1.0) + n;
      // t1 = (tau * y);
      // t2 = ((1 - tau) * ((B * y) * m_amp));
      // y_ = t1 + t2;
      // y  = tanh(y_) + n;

      // cout << "t1 = " << t1 << ", t2 = " << t2 << ", y_ = " << y_ << ", y = " << y << endl;
      float tmp1_, tmp2_;
      tmp1_ =  r(0); // in1[i]; // 0.0;
      for (ri = 1; ri < iwsize/2; ri++)
        tmp1_ +=  r(ri); // in1[i]; // 0.0;
      tmp2_ =  r(ri); // -in1[i]; // 0.0;
      for (; ri < iwsize; ri++)
        tmp2_ +=  r(ri); // in1[i]; // 0.0;
      outbuf1[i] = tmp1_ / sqrt(in_wsize);
      outbuf2[i] = tmp2_ / sqrt(in_wsize);
    }
    // x = vec(x);
    }
    else {
      for (int i = 0; i < inNumSamples; i++) {
        outbuf1[i] =  r(0); // in1[i]; // 0.0;
        outbuf2[i] =  r(1); // in1[i]; // 0.0;
      }
    }

    block_cnt++;
  }
};


PluginLoad(ReservoirUGens) {
  ft = inTable;
  registerUnit<BufReservoir>(ft, "BufReservoir");
}
