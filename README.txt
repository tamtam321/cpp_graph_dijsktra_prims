Leírok pár dolgot, amit fontosnak tartok, hátha segít értelmezni a programomat.

* A main.cpp-ben írtam teszteket, de abban a 6, 7, 11 és a 16-os teszt végét kikommenteltem, mert
ott a hibák kiíratását tesztelem. Exception-t dobnak és emiatt nem halad tovább a teszt, ezért kikommenteltem.
Majd ha azokat nézitek legyetek szívesek kikommentelni.

* Ha txt fájllal szeretnénk felépíteni a gráfot, akkor a fájl legyen
a working directory-ban, ami alapértelmezetten a CLion-ben az a cmake-build-debug.

* Fájlbeolvasással is lehet gráfot felépíteni, de a addVertex 
metódussal is fel lehet építeni. Építés során oda kell figyelni hogy
eddig milyen csúcsok vannak a gráfban, mint amikor papíron próbálunk 
gráfot felrajzolni. Ha olyan csúcsokat akarunk összekötni, ahol az egyik
nem is létezik még, azt nem engedem és hibaüzenetet dobok.

* A csúcsok címkézése char típusú így, ha betűkkel a címkézünk,
akkor csak az angol abc betűi jöhetnek szóba, vagyis amik 
csak 1 karakter hosszú, de a feladatleírásban nem volt külön említve, 
hogy milyen legyen a címkézés, így nem foglalkoztam vele.

* Törlésnél pedig csak olyan csúcsokat engedek törölni, ami nem okoz
izolációt a gráfban.

* Ki lehet iratni a gráfot, ahol az alábbi sorrendben jeleníti meg az adatokat:
csúcs
szomszédok
szomszéddal összekötött él súlya

* Az utolsó teszt direkt rossz. Megnéztem, hogy dob-e exception-t.