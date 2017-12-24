#ifndef ZELEZIAREN_H
#define ZELEZIAREN_H

#define SIMLIB_DAYS(x) (x * 60.0 * 24.0)
#define SIMLIB_HOURS(x) (x * 60.0)
#define SIMLIB_MINUTES(x) (x)
#define SIMLIB_SECONDS(x) (x / 60.0)

const int PRACOVNA_HMOTA_KONTIZLIATKY = 15;
const int KAPACITA_SKLADU = 10000;
const int KAPACITA_NARAZACEJ_PECE = 40;
const int OHREV_KROKY = 113;

const int BARIERA_PEC = 3;
const int BARIERA_OHREV = 7;
const int BARIERA_OHREVNY_BALIK = 4;
const int OHREVNY_BALIK_OHREVNE_BLOKY = 4;
const int KONTIZLIATOK_POCET_BLOKOV = 11;

class PrechodovaBariera : public Barrier {
    using Barrier::Barrier;
    int Break(void);
};

#endif // ZELEZIAREN_H
