#auslesen
#Dieses Skript stellt eine Alternative zum eigentlichen Skript auslesen dar. Grund dafür ist der Absturz des Weblicht Servers am WE vom 20.11.15-22.11.15, der zu einer Alternativensuche geführt hat. Das mit der Alternative einhergehende veränderte Format muss beim Auslesen der Nomen berücksichtigt werden.
#Benötigte Dateien:
#Jahr_ressort.csv
#Jahrausgaben.csv


#Zeitraum
my $Jahr_Start=1969;
my $Jahr_Ende=1969;

#Grundinitalisierung der Variablen
$Jahr=1969;
$Ausgabe=1;
$ressort=1;
$Artikelnummer=1;
$Artikeltag="$Jahr-$Ausgabe-$ressort-$Artikelnummer";
$Urform="";
@Nomen="";
$Nomen="";
$Zahl=0;
$Zahl2=0;
$Ressortname="";

for($Jahr=$Jahr_Start;$Jahr<=$Jahr_Ende;$Jahr++)
	{
	#Schleife Jahrgang
		if(-e "$Jahr.txt")
		{
		
		if(!-e "$Jahr\_analysiert.txt")
			{
			#Part of Speech Tagger laufen lassen
			open(PIPE, "| tag-german.bat $Jahr.txt > $Jahr\_analysiert.txt");
			close(PIPE);
			}
		#Laden der fertig analysierten Datei und schreiben der Daten in eine Variable
		open(DATEI, "$Jahr\_analysiert.txt") || die "Could not read $Jahr\_analysiert.txt !";
		while(<DATEI>) {$Analysiert.=$_;}
		close(DATEI);
		
		#Protokollieren, dass dieser Schritt funktioniert hat
		open(DATEI, "> Protokoll_auslesen.txt");
		print DATEI "Alles geladen\n";
		close(DATEI);


		@Nomen="";
		open(DATEI, ">> Protokoll_auslesen.txt");
		print DATEI "Durchgang Schleife Jahr $Jahr\n";
		close(DATEI);	

			#Wie viele Ausgaben gab es in dem Jahr?
			open(AUSGABENDATEI, "JahrAusgaben.csv");
			while(<AUSGABENDATEI>)
				{
				if($_=~/$Jahr,(.*?);/){$maxAusgabe=int($1);}
				}
			for($Ausgabe=1;$Ausgabe<=$maxAusgabe;$Ausgabe++)
				{

				#Schleife Ausgabe
		
				open(DATEI, ">> Protokoll_auslesen.txt");
				print DATEI "Durchgang Schleife Ausgabe $Ausgabe\n";
				close(DATEI);
				
				#Wie viele Ressorts gab es in dieser Ausgabe?
				open(LISTE, "$Jahr\_ressort.csv");
				while(<LISTE>)
					{
					if($_=~/$Jahr,$Ausgabe,(\d*?),/){$maxressort=$1;}
					}
				close(LISTE);
		
				for($Ressort=1;$Ressort<=$maxressort;$Ressort++)
					{
					#Schleife Ressort
			
					$Ressortname="";
					open(DATEI, "$Jahr\_ressort.csv");
					while(<DATEI>)
						{
						if($_=~/$Jahr,$Ausgabe,$Ressort,(.*?)$/sg){$Ressortname="$1";}
						}
					close(DATEI);
					open(DATEI, ">> Protokoll_auslesen.txt");
					print DATEI "Durchgang Schleife Ressort $Ressort, $Ressortname\n";
					close(DATEI);
					#Artikelzähler auf 1 setzen
					$Artikelnummer=1;
			
					#Ressortnamen auslesen, damit man diesen in die Datei später hineinschreiben kann

			
					while($Analysiert=~/Beginn-$Jahr-$Ausgabe-$Ressort-$Artikelnummer/g)
						{
						#Schleife Artikel
				
						$Artikelinhalt="";
						$AnzahlNomenArtikel=0;
						open(DATEI, ">> Protokoll_auslesen.txt");
						print DATEI "Durchgang Schleife Artikel $Artikelnummer\n";
						close(DATEI);
						#Nun müssen die Nomen herausgesucht werden
				
				
						#Eine Abfrage muss den Anfangs und Endtag einfangen und alle Elemente dazwischen untersuchen
		
						if($Analysiert=~/Beginn-$Jahr-$Ausgabe-$Ressort-$Artikelnummer(.*?)Ende-$Jahr-$Ausgabe-$Ressort-$Artikelnummer/sg){$Artikelinhalt.="$1";}
				
				
						while($Artikelinhalt=~/NN.(\w*?)\s/g)
							{
							push(@Nomen,"$1,$Jahr,$Ausgabe,$Ressortname,$Artikelnummer\n");
							$AnzahlNomenArtikel++;							
							}
					
						#Protokollieren. dass was gefunden wurde für eventuelle Fehlersuche
						open(DATEI, ">> Protokoll_auslesen.txt");
						print DATEI "Insgesamt $AnzahlNomenArtikel Nomen in Artikel $Jahr-$Ausgabe-$Ressort-$Artikelnummer gefunden\n";
						close(DATEI);								
						$Artikelnummer++;
						#Ende Schleife Artikel
						}
			
					#Ende Schleife Ressort
					}
		
				#Ende Schleife Ausgabe
				}
	
			#Noch alphabetisch vorordnen
			@SortierteListe = sort(@Nomen);
	
			open(DATEI, ">$Jahr-Nomen.csv");
			print DATEI "@SortierteListe";
			close(DATEI);
	
			@SortierteListe="";
			@Nomen="";
			
			#Ende Schleife Jahrgang
	}
	#Ende der Kontrollschleife, ob der Band zu dem Jahr in dem Ordner existiert
}
#Ende For Schleife (Jahresschleife)

#Ende auslesen