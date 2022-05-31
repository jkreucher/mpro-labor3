# Labor 3: Elektronischer Würfel mit 7‐Segmentanzeige

MPRO Labor bei Prof. Dr.‐Ing. Jens Altenburg an der TH-Bingen


## Zielstellung:

Nach den ersten Versuchen zur Tastenabfrage und der Ausgabe von Lichtsignalen soll im nächsten Schritt die Frage der Bitmuster vertiefend betrachtet werden. Es lässt sich zeigen, dass die mehr oder weniger komplexen LED‐Anzeigen mit der Ausgabe geeigneter Pattern unkompliziert und effizient zu realisieren sind. Eine 7‐Segment‐Anzeige ist letztendlich ebenfalls nichts anderes als die Anordnung spezifisch geformter Leuchtdioden zur Anzeige von Zahlen zwischen 0 und 9. Mit etwas Phantasie sind auch die Hexadezimalzahlen A bis F mit einer solchen Anzeige darstellbar. Der folgende Laborversuch die Tastenabfrage und die Zahlendarstellung zum Programmieren eines elektronischen Würfels. Das Hauptaugenmerk liegt auf der effizienten Programmierung, dem Einsatz von Bitmustern, der Entprellung „gestörter“ Tastenimpulse und dem Einbau einer Hintertür in die Software zur Manipulation der Würfelergebnisse.

## Aufgabenstellung:

- Zunächst wird ein elektronischer Würfel für klassische Brettspiele programmiert. Dieser soll beim Betätigen des Schiebeschalters SW1 einen „klassischen Holzwürfel“ simulieren. In dieser Simulation wird die Zahlenanzeige alle 50ms umgeschaltet. Die Abfolge der angezeigten Zahlen erfolgt „pseudozufällig“, d.h. nicht in der Abfolge 1, 2, ... 6, sondern in einem von vier vorgegebenen Mustern. Die Auswahl des angezeigten Musters ist zufällig.

- Sobald der Schiebeschalter in die Ruheposition bewegt wird, „rollt“ das Würfelmuster aus. Das bedeutet, dass die Anzeige nach min. 2 bzw. max. 8 Anzeigewerten stabil bleibt. Der Ausrollvorgang erfolgt ebenfalls nach vier zufälligen Zeitmustern. Das erste Zeitraster verlängert die Anzeigezeit in Schritten von 50 ms, d.h. 50, 100, 150, 200, 250, 300, 350, 400 ms. Im zweiten Raster wird in Zeitschritten von 25 ms jeweils vervilefacht – 100+25, 100+50, 100+75, 100+100, 100‐+125, 100+150, 100+175, 100+200 ms. Die letzten beiden Zeitraster sind frei wählbar. Hier muss die Zeit nicht von Position zu Position anwachsen!

- Der elektronische Würfel verfügt über eine Manipulationsfunktion. Wird der Taster SW2 kurz vor SW1 unauffällig betätigt (nicht gleichzeitig mit SW1) werden vorrangig „6‐en“ erzeugt. Der Anteil der „6‐en“ beträgt 50%. Es dürfen aber nicht nur „6‐en“ generiert werden.

- Wird SW5 kurz betätigt werden vorrangig „1‐en“ gewürfelt (Anteil 50%).

- Der „Mogeltrick“ ist zeitbegrenzt. Wird nicht innerhalb von 1,5 Sekunden gewürfelt, erlischt die Manipulationsfunktion automatisch.

- Der Taster SW3 aktiviert die Diagnosefunktion. Wird SW3 beim POR betätigt, erscheint der Dezimalpunkt in der 7‐Segment solange der „Mogelmode“ aktiv ist.
  

## Lösungshinweise:

- Zufallszahlen: Überlegen Sie wie Sie aus der while(1){ ... } Schleife in der main() auf einfache Art und Weise Pseudozufallszahlen erzeugen können.
  
- Zahlenmuster: Nutzen Sie entsprechende Bitmuster (Pattern) die Sie in geeigneter Weise zu Array anordnen, um die Ausgabewerte der Anzeige zu erzeugen.

- Zeitliche Abhängigkeiten: Übertragen Sie die Tabellen von „Pattern“ sinnvoll in zeitliche Tabellen. Die Ausgabe von Zeiten und Mustern ist dann einfache über Indices möglich.