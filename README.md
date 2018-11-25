# Celulární automat - Pohyb lidí v ohraničeném prostoru

- Zdrojový článek
https://ac.els-cdn.com/S0378437101001418/1-s2.0-S0378437101001418-main.pdf?_tid=03729180-3c34-4543-a0e6-59e74e3b7e3a&acdnat=1542982739_cc793283e605f838052d87d4f3f66521

## Základní princip a východiska
- matice MxM, na ni náhodně na začátku simulace rozmístěno x lidí (dále fermionů - viz článek)
  - logicky platí, že x < MxM, nicméně začal bych s nějakým rozumným počtem x < (MxM)/1,5 třeba
- rychlost pohybu fermionů za jednu časovou jednotku je konstantní -> 1

Na pohyb fermionů má vliv několik skutečností:

- STATICKÁ (tedy po celou dobu programu neměnná) 3x3 MATICE PREFERENCÍ směru pohybu fermionů do sousedních buněk
  - ze současného bodu se můžu pohybovat osmi různými směry (plus devátý směr je setrvání na místě) - ilustrace článek str 511 nahoře
  - hodnoty polí matice (M(i,j)) lze stanovit různými způsoby (inspirace např Appendix A článku), to chce ještě poladit (@Adam), ale pro začátek si tam můžeme hodit vcelku cokoli. Musí samozřejmě platit, že suma pravděpodobnosti těchto 9 možných pohybů je 1.
  - Př) Pokud může fermion jít jen jedním směrem (např modelujeme frontu někam...), bude M(0,1) = 1 a všechny zbylé M(i,j)=0
  - ZÁVĚR: Fermion se z pozice v čase t může vydat do pozice v čase t + 1 9 různými směry. Kam se nakonec vydá, je dáno maticí preference.

- POZEMNÍ POLE, jehož účelem je modelovat vlivy interakcí mezi fermiony na delší vzdálenosti. Jeho hodnota v buňkách je modifikována fermiony a fermiony jsou hodnotami pozemního pole v daných buňkách ovlivňovány (konkrétně pravděpodobnosti přechodu do různých směrů)
  - pozemní pole modifikuje pravděp. přechodu tak, že pohyb ve směru větších volných ploch je preferován
  - lze si představit jako druhou vrstvu buněk (o stejném MxM rozměru)
  - máme pozemní pole:
      - STATICKÉ (statická složka pozemního pole) - které je v čase zcela neměnné (tedy neovlivňováno fermiony). "Hodnota intenzity" statického pole je určena počtem tzv. s-bosonů: s-boson pro bod [x,y] je označen jako Ts(x,y). Počty s-bosonů pro všechny body [X,Y] jsou stanoveny fixně na začátku simulace.
        Př) Máme prostor MxM, modelujeme místnost s únikovým východem v případě požáru. Obecně lze říct, že být blíže u východu je fermiony preferovanější, tudíž počty s-bosonů jsou nejvyšší v bodech hned u něj a klesají s rostoucí vzdáleností od východu.

      - DYNAMICKÉ (dynamická složka pozemního pole) - které je naopak přítomností fermionů modifikováno a má svoji vlastní dynamiku. V tomto případě je "intenzita" dynamické složky modelována počty tzv. d-bosonů Td(x,y), kterých je na začátku simulace právě nula ve všech bodech. Jakmile fermion vykoná pohyb z [x,y] do jiné buňky, počet d-bosonů v buňce [x,y] vzroste o 1 ("stopa"...)
        Př) Každý fermion zanechává po sobě "stopu", tzn. dynamická složka pozemního pole (tedy počet d-bosonů) je vyšší u obsazených buněk. Vzhledem k tomu, že celková pravděpodobnost zvolení směru pohybu závisí i na hodnotě dynamické složky pozemního pole, pro ostatní fermiony je žádoucí následovat ostatní fermiony.
        - dynamická složka pozemního pole se časem snižuje (stopy od ostatních fermionů postupem času mizí)
          - => jakmile jsou vykonány všechny přechody z t do t+1, nejstarší d-boson (Td) je z každé buňky odstraněn s pravděpodobností alfa (== vstupní parametr simulace), pokud existuje déle než jeden časový úsek (tj. pokud vznikl během předešlého nebo staršího přechodu)

Tudíž v obecné rovině je pravděpodobnost přechodu p-ij (čti pé s indexem i,j) ve směru (i,j) dána jako:

p-ij = N x M-ij x D-ij x S-ij x ( 1 - n-ij ), kde:

    - n-ij nabývá hodnoty 1 nebo 0 (buňka je obsazená fermionem nebo je prázdná)
    - N je normalizační faktor, který zajištuje, aby suma(p-ij) pro všechny varianty dvojic i,j byla 1
    - M-ij - hodnota matice preferencí (viz výše)
    - D-ij - hodnota dynamické složky pozemního pole
    - S-ij - hodnota statické složky pozemního pole

## Pravidla pohybu fermionů:

- Obecně platí, že cílová buňka pro čas t+1 je určena pro všechny fermiony v čase t paralelně ve stejný okamžik na základě jejich matice preferencí a hodnot statické a dynamické složky pozemního pole cílové buňky (viz vzorec o pár řádků výše)
- v čase t každý fermion ověřuje, které sousední buňky jsou prázdné (hodnota n-ij == 0). Pravděpodobnost přechodu do obsazené buňky jsou nastaveny na nulu.
- pokud m fermionů určí jako svůj cíl stejnou buňku, jenom jeden z nich smí pohyb vykonat. Tento je určen náhodným výběrem, kde každému takovému fermionu je přiřazena hodnota pravděpodobnosti výběru 1/m. Lze použít i jinou sofistikovanější metodu (viz dodatek dole), ale v článku píšou, že jim vyšlo, že nemá žádný zásadní přínos, tudíž zůstáváme u této....
- Hodnota dynamické složky pozemního pole se mění na základě pravidel postupného "mizení stop" - viz popis dynamické složky pozemního pole výše
- Fermiony mění hodnotu dynamické složky pozemního pole v buňce, ve které se nacházeli v čase t+1 před přechodem


Pravděpodobnost přechodu z aktuálního bodu (0,0) (středová buňka) do neobsazeného souseda je tedy:

p-ij = N x exp(beta x J-s x delta-s(i,j)) x exp(beta x J-d x delta-d(i,j)) x (1 - n-ij) x d-ij (viz str 514 nahoře) kde:

  - N == normalizační faktor, viz vzorec výše
  - J-s je "míra vlivu" s-bosonů (statické složky pozemního pole) na pohyb fermionů (=> další vstupní argument simulace)
  - J-d: totéž pro d-bosony (dynamickou složku)
  - delta-s(i,j) je rozdíl v počtu s-bosonů v cílovém bodě a výchozím bodě přechodu (Ts(i,j) - Ts(0,0))
  - delta-d(i,j) totéž pro d-boson
  - beta hraje roli eliminující konstanty (viz bod níže...)
  - d-ij: korekční faktor, který bere v potaz směr, ze kterého fermion přichází (jinými slovy, aby proto, že někde byl, tam nechtěl o to víc jít zpět....)
    - pokud byl fermion v čase t-1 v bodě i, d-boson ležící na vrcholu dynamického pole v bodě i byl zanechán právě tímto fermionem -> nastavíme-li tedy:
      d-i,j = exp( -beta x J-d), nebude tento d-boson brán v potaz při výpočtu pravděpodobností přechodů
    - pokud se v přechodu z t-1 -> t fermion posunul v určitém směru, je potřeba tento pohyb reflektovat i v přechodu t->t+1, tedy "zesílit pravděpodobnost", že bude vybrán tento směr i v tomto přechodu, tedy: d-i,j = exp(beta x J-o). Parametr J-o bude tedy dalším vstupním argumentem simulace, kterým nastavíme jakousi "setrvačnost" fermionů.
    - ve všech ostatních případech platí, že d-i,j = 1


- Dodatek (vcelku irelevantní. Přečíst primárně řešení konfliktů v buňkách, třetí odrážka v pravidlech pohybu fermionů):
    - Je-li pro nějaký fermion vypočtena cílová buňka taková,
        která je aktuálně (v čase t) obsazená, fermion se nehne (viz o jeden bod výše)
        která aktuálně není obsazená a necíluje-li ji pro čas t+1 jiný fermion, pohyb se vykoná.
        která aktuálně není obsazená a cíluje ji pro čas t+1 jeden nebo více fermionů, tento pohyb vykoná jen buňka s nejvyšší relativní pravděp. pohybu tímto směrem (viz vzorec a ilustrace v článku str 511 nahoře). Zbylé fermiony opět zůstávají na svých pozicích.

## Simulace, tipy, inspirace a očekávané hodnoty
- Simulace únikového východu z článku:
  - dovolili fermionům pohybovat se jen 5 směry (tedy ne diagonálně), to bychom si možná taky mohli zjednodušit
  - matice (místnost) nemusí být nutně čtvercová, nicméně oni též zvolili čtvercovou (stejně jako my), takže cajk
  - studovali vliv délky života d-bosonů, konkrétně parametru alfa (s jakou pravděp. se v rámci dané iterace odstraní nejstarší d-boson z buňky....) na celkovou evakuační dobu - tedy za jak dlouho všichni opustí místnost, přičemž "východ" je definován jako tři souvislé body (tedy místnost mohou opustit tři lidi zároveň v rámci jedné iterace, což tak nějak odpovídá i realitě...)
    - -> jinými slovy "konec života fermionu" bych namodeloval tak, že dostane-li se jakýkoli fermion na jeden z těchto tří bodů, v rámci další iterace už se vůbec neřeší jeho preference a všechno ostatní a prostě přestane existovat, opouští systém (a samozřejmě uvolní místo u dveří...)
  - výchozí parametry simulace: J-s = 2; J-d = 10; beta = 10 (takto bych v rámci testování model spouštěl...)
  - ZÁVĚRY (aneb k čemu bychom měli s danými parametry asi ideálně rámcově taky nějak dospět):
      - při vyšší J-s, např. J-s = 2, tedy silnější vazbé fermionů na s-bosony, roste doba potřebná k evakuaci s nárůstem životnosti d-bosonů (== jinými slovy při nízkém alfa, tedy pravděpodobnosti jejich zániku...)
      -  pokud je J-s menší, např. J-s = 0,5, nejlepší evakuační časy jsou při "střední až malé životnosti" d-bosonů, tedy pro alfa okolo 0,55-0,70
      - co z toho všeho lze vyvodit 
        - při intenzivnější vzájemné vazbě mezi fermiony (...menší alfa -> delší životnost průměrného d-bosonu) může vést k zásadním změnám chování fermionů. 
        - Tyto změny jsou ještě zásadnější, když fermiony nemají příliš velké ponětí o tom, jaká je nejlepší cesta k nejbližšímu východu (....menší J-s -> menší vazba fermionů na hodnoty statického pole -> menší ovlivňování jejich následujícího kroku od statického pole...). 
        - Pokud se J-s blíží k nule (tedy pokud fermiony nemají téměř potuchy, kde východ je), tak snížení hodnoty alfa (zvýšení délky života průměrného d-bosonu) z již nízkých hodnot (např z 0,2 na 0,1) vede k drastickému nárůstu evakuační doby (v jejich případě konkrétně z 800 na 2000 časových jednotek) - viz grafy str. 518

- -> simulujeme únikový východ, který odpovídá třem vedle sobě ležícím bodům
- -> pro začátek nastavíme parametry následovně:
  - J-s = 2
  - J-d = 10
  - beta = 10
  - alfa = 0,5 
- -> a budeme si následně hrát se vším ohledně bety, přičemž to, co nás zajímá, je doba potřebná k evakuaci. 

