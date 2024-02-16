MOdulation SEQuencer
--------------------
- *In retrospect*: See the thoughts below (Wed 04 Apr 2018). This repo should
  be viewed as a dump for sequencer experiments, not a project in itself.


--- Partial goals ---
> Very simple sequencer with a gtk gui
> An 'engine' that can read a script file with a certain syntax, parse it, and make a sequence that plays along with jack transport.
> A gtk text editor for sequencer scripts. This can be a module in the final 'product', used for advanced operations that aren't achievable with the GUI.
> Graphical menus that creates the sequencer scripts for the user, making scripting become unnecessary for anything but the most advances operations.


--- IDEAS and TODOS: ---
> Implementér en ekstra metode for å loope små områder, helt ned på periodenivå. Selv om dette også ender opp som en del av "time travel"-tabellen er dette umulig å gjøre for hånd i den opprinnelige gui-ideen. F. eks.: La brukeren kunne trykke noe á la "Make tone", noe som åpner en meny hvor man kan velge et lite område med samples. I denne menyen kan man ha knapper som setter antall samples / frames som skal loopes til en bestemt verdi som tilsvarer en bestemt tone.
> Videre er det mulighet for å kunne lage korte melodier med den bestemte loopen. Implementér gjerne en enkel måte å gjøre dette på. 

> Alternativ: Bruk enkel grafisk redigering for "grov modulering", og ha et slags script-språk for å gjøre ting på et finere nivå, som f.eks. repetering av mindre antall samples. Et alternativ til dette igjen er å la alt kunne styres av scripts (som kan lagres), men ha grafiske grensesnitt som gjør det enkelt å lage scriptene uten å faktisk "kode".

--- Wed 04 Apr 2018 11:17:50 PM CEST ---
1.  First Thoughts about the above ideas:
    A lot of the ideas presented here are just reinventing the wheel.
    Making a synthesis engine from scratch is just spending time on
    creating a crappier version of Csound or SuperCollider.
    Prioritize creating (cli) interfaces that you want to use yourself,
    like *nblinc* cli midi sequencer, and maybe some audio effects with 
    unique controls, like *armoda*.
2.  Second thoughts: Maybe the idea for this project was that it should 
    evolve into a breakcore / glitch drum sequencer with fine adjustable
    parameters. Still not sure if it would do anything Csound can't do, though.
----------------------------------------
