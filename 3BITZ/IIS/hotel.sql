-- Hotel

-- Dávid Bolvanský (xbolva00)
-- Martin Marušiak     (xmarus07)
-- Juraj Ondrej Dúbrava (xdubra03)

drop database xbolva00;
create database xbolva00;
USE xbolva00;

-- - drop ---
DROP TABLE IF EXISTS klient;
DROP TABLE IF EXISTS pobyt;
DROP TABLE IF EXISTS platba;
DROP TABLE IF EXISTS zamestnanec;
DROP TABLE IF EXISTS spravca;
DROP TABLE IF EXISTS zlava;
DROP TABLE IF EXISTS izba;
DROP TABLE IF EXISTS sluzba;
DROP TABLE IF EXISTS sluzba_pobyt;

-- create tables ---
CREATE TABLE klient (
	id_klienta integer NOT NULL AUTO_INCREMENT PRIMARY KEY,
	meno varchar(50) NOT NULL,
	priezvisko varchar(50) NOT NULL,
	mesto varchar(50) NOT NULL,
	ulica varchar(50),
	cislo_domu integer NOT NULL,
	stat varchar(50) NOT NULL,
	psc integer NOT NULL,
	rodne_cislo bigint NOT NULL,
	telefonne_cislo varchar(13),
	email varchar(50),
	login varchar(50) NOT NULL,
	heslo varchar(50) NOT NULL
);

CREATE TABLE pobyt (
	id_pobytu integer NOT NULL AUTO_INCREMENT PRIMARY KEY,
	datum_rezervovania_od datetime,
	datum_rezervovania_do datetime,
	datum_ubytovania_od datetime,
	datum_ubytovania_do datetime,
	pocet_osob integer NOT NULL,
	id_klienta integer NOT NULL,
	id_platby integer,
	id_zamestnanca integer NOT NULL,
	id_zlavy integer NOT NULL,
	cislo_izby integer
);

CREATE TABLE platba (
	id_platby integer NOT NULL AUTO_INCREMENT PRIMARY KEY,
	suma integer NOT NULL,
	datum_platby datetime NOT NULL,
	sposob_platby integer NOT NULL,
	cislo_uctu varchar(24),
	id_klienta integer,
	id_zamestnanca integer NOT NULL
);

CREATE TABLE zamestnanec (
	id_zamestnanca integer NOT NULL AUTO_INCREMENT PRIMARY KEY,
	meno varchar(50) NOT NULL,
	priezvisko varchar(50) NOT NULL,
	titul varchar(50),
	mesto varchar(50) NOT NULL,
	ulica varchar(50),
	cislo_domu integer NOT NULL,
	psc integer NOT NULL,
	rodne_cislo bigint NOT NULL,
	telefonne_cislo varchar(13),
	email varchar(50),
	cislo_uctu varchar(24) NOT NULL,
	login varchar(50) NOT NULL,
	heslo varchar(50) NOT NULL
);

CREATE TABLE spravca (
	id_zamestnanca integer NOT NULL AUTO_INCREMENT PRIMARY KEY
);

CREATE TABLE zlava (
	id_zlavy integer NOT NULL AUTO_INCREMENT PRIMARY KEY,
	nazov varchar(50) NOT NULL,
	rezervacna_zlava integer NOT NULL,
	sezonna_zlava integer NOT NULL
);

CREATE TABLE izba (
	cislo_izby integer AUTO_INCREMENT PRIMARY KEY,
	kapacita integer NOT NULL,
	pocet_miestnosti integer NOT NULL,
	dostupnost integer NOT NULL,
	cena integer NOT NULL
) AUTO_INCREMENT = 100;

CREATE TABLE sluzba (
	id_sluzby integer NOT NULL AUTO_INCREMENT PRIMARY KEY,
	nazov varchar(50) NOT NULL,
	typ integer NOT NULL,
	cena integer NOT NULL
);

CREATE TABLE sluzba_pobyt (
	id_sluzby integer NOT NULL,
	id_pobytu integer NOT NULL
);

-- - set composite primary keys ---
ALTER TABLE sluzba_pobyt ADD CONSTRAINT pk_id_sluzby_id_pobytu PRIMARY KEY (id_sluzby, id_pobytu);

-- - set foreign keys ---
ALTER TABLE sluzba_pobyt ADD CONSTRAINT fk_id_sluzby FOREIGN KEY (id_sluzby) REFERENCES sluzba(id_sluzby);
ALTER TABLE sluzba_pobyt ADD CONSTRAINT fk_id_pobytu FOREIGN KEY (id_pobytu) REFERENCES pobyt(id_pobytu);

ALTER TABLE spravca ADD CONSTRAINT fk_id_zamestnanca FOREIGN KEY (id_zamestnanca) REFERENCES zamestnanec(id_zamestnanca);

ALTER TABLE platba ADD CONSTRAINT fk_id_zamestnanca_2 FOREIGN KEY (id_zamestnanca) REFERENCES zamestnanec(id_zamestnanca);
ALTER TABLE platba ADD CONSTRAINT fk_id_klienta FOREIGN KEY (id_klienta) REFERENCES klient(id_klienta);

ALTER TABLE pobyt ADD CONSTRAINT fk_id_zamestnanca_3 FOREIGN KEY (id_zamestnanca) REFERENCES zamestnanec(id_zamestnanca);
ALTER TABLE pobyt ADD CONSTRAINT fk_id_klienta_2 FOREIGN KEY (id_klienta) REFERENCES klient(id_klienta);
ALTER TABLE pobyt ADD CONSTRAINT fk_id_platby FOREIGN KEY (id_platby) REFERENCES platba(id_platby);
ALTER TABLE pobyt ADD CONSTRAINT fk_id_zlavy FOREIGN KEY (id_zlavy) REFERENCES zlava(id_zlavy);
ALTER TABLE pobyt ADD CONSTRAINT fk_cislo_izby FOREIGN KEY (cislo_izby) REFERENCES izba(cislo_izby);


-- - insert values ---
INSERT INTO klient(meno, priezvisko, mesto, ulica, cislo_domu, stat, psc, rodne_cislo, telefonne_cislo, email, login, heslo) VALUES('Jan','Novak', ' Brno', 'Purkynova', 27, 'Ceska republika', 61200, 8605102528, '+420975684755', 'jan.novak@seznam.cz', 'jnovak', 'bm92YWsyag==');
INSERT INTO klient(meno, priezvisko, mesto, ulica, cislo_domu, stat, psc, rodne_cislo, telefonne_cislo, email, login, heslo) VALUES('Petr','Franko', ' Praha', 'Hlavni', 42, 'Ceska republika', 14100, 8009036475, '+420971855653', 'frankopetr@google.com', 'pfranko', 'ZnJhbmtvMnA=');
INSERT INTO klient(meno, priezvisko, mesto, ulica, cislo_domu, stat, psc, rodne_cislo, telefonne_cislo, email, login, heslo) VALUES('Jana','Frankova', ' Praha', 'Hlavni', 42, 'Ceska republika', 14100, 8408213616, '+420653244156', 'jankafrankova@google.com', 'jfrankova', 'ZnJhbmtvdmEyag==');
INSERT INTO klient(meno, priezvisko, mesto, ulica, cislo_domu, stat, psc, rodne_cislo, telefonne_cislo, email, login, heslo) VALUES('Katerina','Mala', ' Olomouc', 'Polska', 13, 'Ceska republika', 77900, 8802259576, '+420657877459', 'katerina.mala@outlook.com', 'kmala', 'bWFsYTJr');

INSERT INTO zamestnanec(meno, priezvisko, titul, mesto, ulica, cislo_domu, psc, rodne_cislo, telefonne_cislo, email, cislo_uctu, login, heslo) VALUES('Filip','Dula', 'Ing.', 'Brno', 'Purkynova', 25, 60200, 9708026119, '+420985677113', 'filip.dula@gmail.com', 'CZ2145563556213524563664', 'pdula', 'ZHVsYTJw');
INSERT INTO zamestnanec(meno, priezvisko, titul, mesto, ulica, cislo_domu, psc, rodne_cislo, telefonne_cislo, email, cislo_uctu, login, heslo) VALUES('Richard','Mraz', NULL, 'Brno', 'Nerudova', 12, 60200, 8606276536, '+420322651428', 'mraz.richard@gmail.com', 'CZ2145563556213524563664', 'rmraz', 'bXJhejJy');
INSERT INTO zamestnanec(meno, priezvisko, titul, mesto, ulica, cislo_domu, psc, rodne_cislo, telefonne_cislo, email, cislo_uctu, login, heslo) VALUES('Petr','Kral', 'Ing.', 'Brno', 'Purkynova', 64, 61200, 8305147785, '+420567845943', 'petr.kral@gmail.com', 'CZ1489963546214523562544', 'pkral', 'a3JhbDJw');
INSERT INTO zamestnanec(meno, priezvisko, titul, mesto, ulica, cislo_domu, psc, rodne_cislo, telefonne_cislo, email, cislo_uctu, login, heslo) VALUES('Eliska','Fernerova', 'Mgr.', 'Brno', 'Minska', 75, 61600, 8806276985, '+420156211457', 'fernerovaeliska@gmail.com', 'CZ1578445678321564872214', 'efernerova', 'ZmVybmVyb3ZhMmU=');

INSERT INTO platba(suma, datum_platby,  sposob_platby, cislo_uctu, id_klienta, id_zamestnanca) VALUES(0, STR_TO_DATE('01-01-2017', '%d-%m-%Y '), 2, NULL, NULL, 3);
INSERT INTO platba(suma, datum_platby,  sposob_platby, cislo_uctu, id_klienta, id_zamestnanca) VALUES(1350, STR_TO_DATE('10-03-2017', '%d-%m-%Y'), 1, 'CZ2465432568942531254521', 4, 2);
INSERT INTO platba(suma, datum_platby,  sposob_platby, cislo_uctu, id_klienta, id_zamestnanca) VALUES(1450, STR_TO_DATE('11-03-2017', '%d-%m-%Y'), 2, NULL, 1, 3);
INSERT INTO platba(suma, datum_platby,  sposob_platby, cislo_uctu, id_klienta, id_zamestnanca) VALUES(1250, STR_TO_DATE('18-03-2017', '%d-%m-%Y'), 1, 'CZ2452642516413652445614', 3, 2);
INSERT INTO platba(suma, datum_platby,  sposob_platby, cislo_uctu, id_klienta, id_zamestnanca) VALUES(1750, STR_TO_DATE('24-03-2017', '%d-%m-%Y'), 2, NULL, 1, 1);


INSERT INTO zlava(nazov, rezervacna_zlava, sezonna_zlava) VALUES('Len rezervacna', 1, 0);
INSERT INTO zlava(nazov,rezervacna_zlava, sezonna_zlava) VALUES('Bez zlavy', 0, 0);
INSERT INTO zlava(nazov,rezervacna_zlava, sezonna_zlava) VALUES('Obe zlavy', 1, 1);
INSERT INTO zlava(nazov,rezervacna_zlava, sezonna_zlava) VALUES('Len sezonna', 0, 1);

INSERT INTO sluzba(nazov, typ, cena) VALUES('Masaz', 2, 200);
INSERT INTO sluzba(nazov, typ, cena) VALUES('Kozmetika', 2, 150);
INSERT INTO sluzba(nazov, typ, cena) VALUES('Baby sitting', 1, 300);
INSERT INTO sluzba(nazov, typ, cena) VALUES('Bazen', 4, 150);

INSERT INTO izba(kapacita, pocet_miestnosti, dostupnost, cena) VALUES(4, 4, 1, 650);
INSERT INTO izba(kapacita, pocet_miestnosti, dostupnost, cena) VALUES(2, 3, 1, 550);
INSERT INTO izba(kapacita, pocet_miestnosti, dostupnost, cena) VALUES(3, 3, 0, 600);
INSERT INTO izba(kapacita, pocet_miestnosti, dostupnost, cena) VALUES(2, 2, 1, 400);
INSERT INTO izba(kapacita, pocet_miestnosti, dostupnost, cena) VALUES(4, 3, 0, 500);
INSERT INTO izba(kapacita, pocet_miestnosti, dostupnost, cena) VALUES(3, 3, 1, 450);
INSERT INTO izba(kapacita, pocet_miestnosti, dostupnost, cena) VALUES(2, 4, 1, 650);
INSERT INTO izba(kapacita, pocet_miestnosti, dostupnost, cena) VALUES(4, 3, 1, 550);
INSERT INTO izba(kapacita, pocet_miestnosti, dostupnost, cena) VALUES(3, 3, 1, 600);
INSERT INTO izba(kapacita, pocet_miestnosti, dostupnost, cena) VALUES(3, 2, 1, 400);
INSERT INTO izba(kapacita, pocet_miestnosti, dostupnost, cena) VALUES(2, 3, 0, 500);
INSERT INTO izba(kapacita, pocet_miestnosti, dostupnost, cena) VALUES(2, 3, 1, 450);
INSERT INTO izba VALUES(1000, 50, 1, 0, 150);

INSERT INTO pobyt(datum_rezervovania_od, datum_rezervovania_do, datum_ubytovania_od, datum_ubytovania_do, pocet_osob, id_klienta, id_platby, id_zamestnanca, id_zlavy, cislo_izby) VALUES(NULL, NULL, STR_TO_DATE('02-03-2017', '%d-%m-%Y'), STR_TO_DATE('05-03-2017', '%d-%m-%Y'), 3, 2, 2, 3, 2, NULL);
INSERT INTO pobyt(datum_rezervovania_od, datum_rezervovania_do, datum_ubytovania_od, datum_ubytovania_do, pocet_osob, id_klienta, id_platby, id_zamestnanca, id_zlavy, cislo_izby) VALUES(STR_TO_DATE('04-03-2017', '%d-%m-%Y'), STR_TO_DATE('09-03-2017', '%d-%m-%Y'), STR_TO_DATE('04-03-2017', '%d-%m-%Y'), STR_TO_DATE('09-03-2017', '%d-%m-%Y'), 2, 2, 2, 2, 1, 104);
INSERT INTO pobyt(datum_rezervovania_od, datum_rezervovania_do, datum_ubytovania_od, datum_ubytovania_do, pocet_osob, id_klienta, id_platby, id_zamestnanca, id_zlavy, cislo_izby) VALUES(NULL, NULL, STR_TO_DATE('18-03-2017', '%d-%m-%Y'), STR_TO_DATE('24-03-2017', '%d-%m-%Y'), 3, 1, 3, 4, 3, NULL);
INSERT INTO pobyt(datum_rezervovania_od, datum_rezervovania_do, datum_ubytovania_od, datum_ubytovania_do, pocet_osob, id_klienta, id_platby, id_zamestnanca, id_zlavy, cislo_izby) VALUES(STR_TO_DATE('05-03-2017', '%d-%m-%Y'), STR_TO_DATE('12-03-2017', '%d-%m-%Y'), STR_TO_DATE('05-03-2017', '%d-%m-%Y'), STR_TO_DATE('12-03-2017', '%d-%m-%Y'), 1, 4, 1, 1, 4, 102);


INSERT INTO sluzba_pobyt VALUES(1, 1);
INSERT INTO sluzba_pobyt VALUES(2, 2);
INSERT INTO sluzba_pobyt VALUES(3, 4);

INSERT INTO spravca VALUES(3);

COMMIT;
