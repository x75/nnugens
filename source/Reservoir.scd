s.boot;
PV_BinWipe


(
s.quit;
{s.boot;}.defer(1);
//{s.scope;}.defer(3);
)


( // BufReservoir: put params in buffer to be passed
var c1, c2;
// the buffer
a = FloatArray.fill(9 * 12, {1.0.rand2}); // 5 seconds of noise
["a", a].postln;
b = Buffer.loadCollection(s, a);
// b = Buffer.loadCollection(s, a);
c1 = Array.fill(9 * 12, {10.0.rand2.floor});
c2 = Array.fill(9 * 12, {10.0.rand2.floor});
//["c1", c1, "c2", c2].postln;
c = [c1, c2];
c = c.lace(9 * 12 * 2);
c.postln;
d = Buffer.loadCollection(s, c, 2);

// // a.plot(discrete: true);
//b.plot;
//d.plot;
)
(
{
	BufReservoir.ar(
		bufW: b,
		bufWloc: d,
		insize: 1,
		in: SinOsc.ar(freq: 583, phase: 0, mul: 1.5, add: 0),
		wsize: 100,
		g: 0.99,
		tau: 0.1
	)
}.play;
)


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