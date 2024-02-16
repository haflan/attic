sr = 44100
ksmps = 32
nchnls = 2
0dbfs  = 1

gisine ftgen   1,   0, 8192,    10, 1            ; Sine

instr 1, AlternativeName
; example annotations:
    ; p# (type, min, max, [default?]), description
    ; p4 (midinn, 60, 90)
    ; p5 (float, 0.0, 1.5), velocity 

  icps     init      cpsmidinn(p4)               ; Get target pitch from score event
  iportime init      abs(p3)/7                   ; Portamento time dep on note length
  iamp0    init      0.25                        ; Set default amps
  iamp1    init      0.25
  iamp2    init      0.25
      
  itie     tival                                 ; Check if this note is tied,
  if itie  ==  1     igoto nofadein              ; if not fade in
  iamp0    init      0

nofadein:
  if p3    < 0       igoto nofadeout             ; Check if this note is held, if not fade out
  iamp2    init      0    

nofadeout:
  ; Now do amp from the set values:
  kamp     linseg    iamp0, .03, iamp1, abs(p3)-.03, iamp2
        
  ; Skip rest of initialization on tied note:
           tigoto    tieskip

  kcps     init      icps                        ; Init pitch for untied note
  kcps     port      icps, iportime, icps        ; Drift towards target pitch

  kpw      oscil     .4, rnd(1), 1, rnd(.7)      ; A simple triangle-saw oscil
  ar       vco       kamp, kcps, 3, kpw+.5, 1, 1/icps
        
  ; (Used in testing - one may set ipch to cpspch(p4+2)
  ;       and view output spectrum)
  ;       ar oscil kamp, kcps, 1

          outs        ar, ar

tieskip:                                        ; Skip some initialization on tied note

endin
