s.boot;
PV_BinWipe

s.scope;
s.freqscope;

s.freeAll;

(
s.quit;
{s.boot;}.defer(1);
//{s.scope;}.defer(3);
)


( // BufReservoir: put params in buffer to be passed
var c1, c2;
var c_mat_randn;
var ressize;
//var ressize; = 10;

c_mat_randn = {|rows = 10, cols = 10, density = 0.1|
	var gaussian_sparse_f = {|density| if(density.coin, {gauss(0, 1.0)}, {0.0})};
	var a1 = Array.fill(rows * cols, {|i| gaussian_sparse_f.value(density)});//.reshape(ressize, ressize);
	//a1.postln;
	a1
};

ressize = 60;
"ressize = " ++ ressize.postln;

// the buffer
//a = FloatArray.fill(9 * 12, {1.0.rand2}); // 5 seconds of noise
a = c_mat_randn.value(ressize, ressize, 0.3);
["a", a].postln;
b = Buffer.loadCollection(s, a);
// // b = Buffer.loadCollection(s, a);
// c1 = Array.fill(9 * 12, {10.0.rand2.floor});
// c2 = Array.fill(9 * 12, {10.0.rand2.floor});
// //["c1", c1, "c2", c2].postln;
// c = [c1, c2];
// c = c.lace(9 * 12 * 2);
// c.postln;
// d = Buffer.loadCollection(s, c, 2);
// ["b,d bufnums", b.bufnum, d.bufnum].postln;
// // a.plot(discrete: true);
//b.plot;
//d.plot;

SynthDef("res", {
	|g = 1.5, tau = 1e-2|
	var env = Env([0.0, 1.0, 1.0, 0.1, 0.0],
		[0.01, 0.95, 0.04], -4);
    var envgen = EnvGen.ar(env, Impulse.ar(0), timeScale: 3.0, doneAction: 2);
	var res = BufReservoir.ar(
		buf: b,
		in: SinOsc.ar(freq: 4.3, phase: 0, mul: 0.001, add: 0),
		insize: 1,
		wsize: ressize,
		tau: tau, // XLine.kr(1e-6, 0.1, 60), // 0.001,
		g: g, //2.0, // 2.0,
		mul: envgen,
	);
	Out.ar(0, res);
}).send(s);

t = Task({
	var g_, tau_;
	inf.do({
		|i|
		g_ = 2.0.exprand(5.0); //exprand;
		tau_ = 0.001.exprand(0.3);
		["g", g_, "tau", tau_].postln;
		x = Synth(\res, [\g, g_, \tau, tau_]);
		5.wait;
		x.free;
	});
});

t.start();

//x = Synth(\res);


// {
// 	BufReservoir.ar(
// 		buf: b,
// 		in: SinOsc.ar(freq: 4.3, phase: 0, mul: 0.001, add: 0),
// 		insize: 1,
// 		wsize: ressize,
// 		g: 2.0,
// 		tau: 0.001,
// 	)
// }.play;
//bufWloc: d,
)

x.set(\g, 3.0)
x.set(\tau, 0.1)

x.free;
t.stop;
s.scope

s.freeAll;


l = Signal.sineFill(16384, Array.fill(200, {0}).add(1));
r = Array.fill(16384, {1.0.rand2});
m = [Array.newFrom(l), r]; // a multi-dimensional array
m = m.lace(32768); // interleave the two collections
b = Buffer.loadCollection(s, m, 2, {|buf|
    x = { PlayBuf.ar(2, buf, BufRateScale.kr(buf), loop: 1) * 0.5 }.play;
});

{BufReservoir.ar}.play

0.7.floor

Array.fill([2, 2, 3], { arg i, j, k;  i * 100 + (j * 10) + k });
//0.1.coin

if(0.1.coin, {"hit".postcs}, {"none".postcs;});
({gauss(0, 1.0)} ! 1000).plotHisto(20, -5, 5);
({1.0.sum3rand} ! 100)

//gauss(0, 1)

(
// image testing
var ressize = 100;
// var a1 = Array.fill(ressize * ressize, {1.rand2;});//.reshape(ressize, ressize);
var gaussian_sparse_f = {|i| if(0.1.coin, {gauss(0, 1.0)}, {0.0})};
var a1 = Array.fill(ressize * ressize, {|i| gaussian_sparse_f.value});//.reshape(ressize, ressize);
var a2 = Int32Array.newFrom(a1 * 100);
["a1.data ", a1].postln;
["a1.shape", a1.shape].postln;
i = Image.new(100, 100);
i.pixels().shape.postln;
i.setPixels(a2);
i.plot;
//i.loadPixels(a1);
)