<CsoundSynthesizer>

<CsOptions>
-d -o dac -m0

</CsOptions>

<CsInstruments>
sr     = 44100
ksmps  = 100
nchnls = 2
0dbfs  = 1

instr 1
    iveloc  = 0.01
    inote   = cpsmidinn(p4)
    kenv    expon iveloc, 0.8, 0.001
    adel    delayr 0.5
    asig    vco2 kenv, inote
    aout  = asig + 0.3*adel
            delayw aout
            outs aout, aout
endin

</CsInstruments>

<CsScore>
f 0 14400    ; a 4 hours session should be enough
</CsScore>
</CsoundSynthesizer>
