#include "simlib.h"
#include <ctime>

#include "zeleziaren.h"

bool oceliaren_prerusenie = false;
bool valcovna_prerusenie = false;

int pocet_kontizliatkov = 0;
int pocet_rur = 0;

Facility kos{"Sadzkovy kos"};

PrechodovaBariera bariera_pec{"Bariera u karusolovej pece", BARIERA_PEC};
PrechodovaBariera bariera_ohrev{"Bariera pred ohrevom", BARIERA_OHREV};
PrechodovaBariera bariera_balik{"Bariera pred vznikom ohrevneho baliku", BARIERA_OHREVNY_BALIK};

Store sklad{"Sklad kontizliatkov", KAPACITA_SKLADU};
Store narazacia_pec{"Narazacia pec", KAPACITA_NARAZACEJ_PECE};
Store karuselova_pec{"Karuselova prec", OHREV_KROKY};

class PracovnaHmota : public Process {
    void Behavior(void) {
        // v panve
        Wait(Uniform(SIMLIB_MINUTES(40), SIMLIB_MINUTES(50)));
        // na otocnom stojane
        Wait(Uniform(SIMLIB_MINUTES(8), SIMLIB_MINUTES(12))); 
        // v medzipanve
        Wait(Uniform(SIMLIB_MINUTES(55), SIMLIB_MINUTES(65)));
        // krystalizatory
        Wait(Uniform(SIMLIB_MINUTES(45), SIMLIB_MINUTES(60)));
        // kontizliatky do skladu
		Enter(sklad, PRACOVNA_HMOTA_KONTIZLIATKY);
        
        pocet_kontizliatkov += PRACOVNA_HMOTA_KONTIZLIATKY;
    }
};

// Bariera, cakanie na N procesov
// Po prelomeni bariery sa neudeje nic (bez reaktivacie cakajucich procesov)
int PrechodovaBariera::Break(void) {
        Clear();
}

// Prvotne naplnenie skladu
class NaplnenieSkladu : public Process {
    void Behavior(void) {
        Enter(sklad, KAPACITA_SKLADU/2);
    }
};

// Davka srotu je 20 ton srotu
class DavkaSrotu : public Process {
    void Behavior(void) {
        // pred vstupom do kosa
        Seize(kos);
        // v kosi
        Wait(SIMLIB_MINUTES(20));
        // uvolnime kos, cakame kym nemame 60 ton (3 davky)
        Release(kos);
        // cakame na tri davky
        bariera_pec.Wait();
        // odliatie, odpich
        Wait(SIMLIB_MINUTES(2));
        (new PracovnaHmota)->Activate();
    }
};

class GeneratorDavkySrotu : public Event {
    void Behavior(void) {
        if (!oceliaren_prerusenie) {
            // dodavaj srot ak nie je prerusenie
            (new DavkaSrotu)->Activate();
        }
        Activate(Time+SIMLIB_MINUTES(20));
    }
};


class PrerusenieVOceliarni : public Process {
    void Behavior(void) {
        oceliaren_prerusenie = true;
        Wait(Uniform(SIMLIB_DAYS(3), SIMLIB_DAYS(6)));
        oceliaren_prerusenie = false;
    }
};

class GeneratorPreruseniaVOceliarni : public Event {
    void Behavior(void) {
        (new PrerusenieVOceliarni)->Activate();
        Activate(Time+Uniform(SIMLIB_DAYS(18), SIMLIB_DAYS(22)));
        
    }
};

class Rura : public Process {
    void Behavior(void) {
        // kontrola rury
        Wait(SIMLIB_MINUTES(5));
        if (Random() <= 0.2) {
            // poskodene
            return;
        }
       
        pocet_rur++;
    }
};

class OhriatyBlok : public Process {
    void Behavior(void) {
         // kalibrovanie, dierovanie, elongovanie, lupy, odvalcovanie
         Wait(SIMLIB_SECONDS(5 * 20));
         // rezanie dna
         Wait(SIMLIB_SECONDS(1));
         // ohrev
         Wait(SIMLIB_HOURS(1));
         // redukovanie
         Wait(SIMLIB_SECONDS(20));
         // chladnutie
         Wait(Uniform(SIMLIB_MINUTES(25), SIMLIB_MINUTES(35)));

         if (Random() <= 0.2) {
            (new Rura)->Activate();
         } else {
             // ohriaty blok sa deli na dva v 80% pripadov
            (new Rura)->Activate();
            (new Rura)->Activate();
         }
    }
};

// 4 bloky spolu
class OhrevnyBalik : public Process {
    void Behavior(void) {
         // vstup do karuslovej pece
         Enter(karuselova_pec);
         // ohrev v peci
         Wait(SIMLIB_HOURS(2.5));
         // odchod z pece
         Leave(karuselova_pec);
         for (int i = 0; i < OHREVNY_BALIK_OHREVNE_BLOKY; ++i) {
             (new OhriatyBlok)->Activate();
         }
    }
};

class Blok : public Process {
    void Behavior(void) {
        // cakame na styri 
        bariera_balik.Wait();
        // vznik baliku
        (new OhrevnyBalik)->Activate();
    }
};

class Kontizliatok : public Process {
    void Behavior(void) {
        // vstup do pece
        Enter(narazacia_pec);
        // ohrev
        Wait(SIMLIB_SECONDS(220));
        // odchod z pece
        Leave(narazacia_pec);
        // delenie
        Wait(SIMLIB_SECONDS(8));
        
        for (int i = 0; i < KONTIZLIATOK_POCET_BLOKOV; ++i) {
            (new Blok)->Activate();
        }
    }
};

class GeneratorKontizliatkov : public Event {
    void Behavior(void) {
        if (!valcovna_prerusenie && sklad.Used() > 0) {
            // zobratie kontizliatku zo skladu
            sklad.Leave(1);
            (new Kontizliatok)->Activate();
        } 
        Activate(Time+SIMLIB_SECONDS(220));
        
    }
};

class PrerusenieVoValcovni : public Process {
    void Behavior(void) {
        valcovna_prerusenie = true;
        Wait(Uniform(SIMLIB_DAYS(5), SIMLIB_DAYS(7)));
        valcovna_prerusenie = false;
    }
};

class GeneratorPreruseniaVoValcovni : public Event {
    void Behavior(void) {
        (new PrerusenieVoValcovni)->Activate();
        Activate(Time+Uniform(SIMLIB_DAYS(18), SIMLIB_DAYS(22)));
        
    }
};

int main(int argc, char *argv[]) {
    RandomSeed(time(nullptr));

    int doba = 7; // v dnoch
    if (argc > 1) {
        doba = atoi(argv[1]);
    }

    Init(0, SIMLIB_DAYS(doba));
    
    (new NaplnenieSkladu)->Activate();
    (new GeneratorDavkySrotu)->Activate();
    (new GeneratorPreruseniaVOceliarni)->Activate();
    (new GeneratorKontizliatkov)->Activate();
    (new GeneratorPreruseniaVoValcovni)->Activate();
    
    
    Run();

    Print("Simulacna doba: %d dni\n", doba);
    Print("Kusy\tTony\n", doba);
    Print("%d\t%d\n", pocet_kontizliatkov, (int)(pocet_kontizliatkov*2.552*(1/0.7)));
    Print("%d\t%d\n", (int)(pocet_rur*0.55), (int)((319*0.72*pocet_rur*0.55)/1000));

    sklad.Output();
    kos.Output();
    narazacia_pec.Output();
}
