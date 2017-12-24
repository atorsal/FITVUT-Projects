<?php

namespace App\Presenters;

use Nette;
use Nette\Application\UI;
use Nette\Database\ForeignKeyConstraintViolationException;
use Nette\Database\UniqueConstraintViolationException;
use Vodacek\Forms\Controls\DateInput;
use \Datetime;


/**
* Sprava pobytov
*/
class AccomodationPresenter extends BasePresenter
{
	private $database;
	private $accomodation;
	private $id_pobytu;

    public function __construct(Nette\Database\Context $database)
    {
        $this->database = $database;
	}
	
	/* Vymazanie pobytu z db */
	public function actionDelete($id) {
		$this->accomodation = $this->database->table("pobyt")->where('id_pobytu', $id)->fetch();
		if (!$this->accomodation) {
			$this->redirect("add");
		}

		try {
			$this->accomodation->delete();
			$this->flashMessage('Pobyt bol odstránený zo systému.', 'ok');
		} catch (ForeignKeyConstraintViolationException $ex) {
			$this->flashMessage('Na pobyt sa viaže platba.', 'fail');
		}
	}
	
	/* Uprava pobytu */
	public function actionEdit($id) {
		$this->accomodation =  $this->database->table("pobyt")->where('id_pobytu', $id)->fetch();
		if (!$this->accomodation) {
			$this->redirect("add");
		}
		
		$this->id_pobytu = $id;
	}
	
	/* Pridanie sluzby k pobytu */
	public function actionAddService($id) {
		$this->actionEdit($id);
	}
	
	/* Odstranenie sluzby pre pobyt */
	public function actionDeleteService($id) {
		$this->actionEdit($id);
	}
	
	/* Zaplatenie za pobyt */
	public function actionPay($id) {
		$this->actionEdit($id);
	}

	/* Ubytovať sa */
	public function actionAccomodate($id) {
		$this->actionEdit($id);
	}
	
	/* Formular na upravu pobytu */
	protected function createComponentEditForm()
    {
		$form = new UI\Form;
		$form->addDate('datum_rezervovania_od', 'Dátum začiatku rezervácie', DateInput::TYPE_DATE)->setRequired(False)->addRule(UI\Form::VALID);
		$form->addDate('datum_rezervovania_do', 'Dátum konca rezervácie', DateInput::TYPE_DATE)->setRequired(False)->addRule(UI\Form::VALID);
		$form->addDate('datum_ubytovania_od', 'Dátum začiatku ubytovania', DateInput::TYPE_DATE)->setRequired(False)->addRule(UI\Form::VALID);
		$form->addDate('datum_ubytovania_do', 'Dátum konca ubytovania', DateInput::TYPE_DATE)->setRequired(False)->addRule(UI\Form::VALID);
		$form->addText('pocet_osob', 'Počet osôb (*):')->setRequired()->addRule(UI\Form::INTEGER, 'Počet osôb musí byť číslo');
		
		
		$clients = null;
		foreach ($this->database->query('select id_klienta, meno, priezvisko from klient')->fetchAll() as $row) {
			$clients[$row->id_klienta] = $row->meno . " " . $row->priezvisko;
		}
		$form->addSelect('id_klienta', 'Klient (*):', $clients)->setRequired();

		$employees = null;
		foreach ($this->database->query('select id_zamestnanca, meno, priezvisko from zamestnanec')->fetchAll() as $row) {
			$employees[$row->id_zamestnanca] = $row->meno . " " . $row->priezvisko;
		}
		$form->addSelect('id_zamestnanca', 'Zaevidoval/a (*):', $employees)->setRequired();

		$discounts = null;
		foreach ($this->database->query('select id_zlavy, nazov from zlava')->fetchAll() as $row) {
			$discounts[$row->id_zlavy] = $row->nazov;
		}
		$form->addSelect('id_zlavy', 'Zľava (*):', $discounts)->setRequired();

		$payments = null;
		foreach ($this->database->query('select id_platby, suma from platba')->fetchAll() as $row) {
			$payments[$row->id_platby] = $row->suma;
		}
		$form->addSelect('id_platby', 'Platba (*):', $payments)->setRequired();

		$rooms = null;
		foreach ($this->database->query('select cislo_izby, kapacita, dostupnost from izba')->fetchAll() as $row) {
			$rooms[$row->cislo_izby] = $row->cislo_izby . " (" . (($row->dostupnost == 1) ? "dostupná" : "nedostupná") . ", kapacita: " . $row->kapacita . ")";
		}
		$form->addSelect('cislo_izby', 'Izba (*):', $rooms)->setRequired();
		
		
		$form->addSubmit('confirm', 'Potvrdiť zmeny');
		$form->onSuccess[] = [$this, 'editFormSucceeded'];
		
		$form->setDefaults([
			'datum_rezervovania_od' => $this->accomodation->datum_rezervovania_od,
			'datum_rezervovania_do' => $this->accomodation->datum_rezervovania_do,
			'datum_ubytovania_od' => empty($this->accomodation->datum_ubytovania_od) ? date('Y-m-d H:i:s') : $this->accomodation->datum_ubytovania_od,
			'datum_ubytovania_do' => $this->accomodation->datum_ubytovania_do,
			'pocet_osob' =>  $this->accomodation->pocet_osob,
			'cislo_izby' =>  $this->accomodation->cislo_izby,
			'id_klienta' => $this->accomodation->id_klienta,
			'id_zamestnanca' =>$this->accomodation->id_zamestnanca,
			'id_zlavy' => $this->accomodation->id_zlavy,
			'id_platby' => $this->accomodation->id_platby
		]);
        return $form;
    }
	
	/* Akcia po potvrdeni zmien u pobytu, aktualizacia db */
    public function editFormSucceeded(UI\Form $form, $values)
    {

		if (!empty($values->datum_ubytovania_od) && !empty($values->datum_ubytovania_do)) {
			$date1 = $values->datum_ubytovania_do;
			$date2 = $values->datum_ubytovania_od;
			$interval = $date2->diff($date1);

			if ($interval->format("%r%a") <= 0) {
			    $this->flashMessage('Neplatný dátum konca ubytovania. Koniec ubytovania musí byť neskorší než začiatok ubytovania. Pobyt nebol upravený v systéme.', 'fail');
				return;
			}
		}

		if (!empty($values->datum_rezervovania_od) && !empty($values->datum_rezervovania_do)) {
			$date1 = $values->datum_rezervovania_do;
			$date2 = $values->datum_rezervovania_od;
			$interval = $date2->diff($date1);

			if ($interval->format("%r%a") <= 0) {
				$this->flashMessage('Neplatný dátum konca rezervácie. Koniec rezervácie musí byť neskorší než začiatok rezervácie. Pobyt nebol upravený v systéme.', 'fail');
				return;
			}
		}

		$this->database->query('UPDATE pobyt SET', [
			'datum_rezervovania_od' => empty($values->datum_rezervovania_od) ? NULL : $values->datum_rezervovania_od,
			'datum_rezervovania_do' => empty($values->datum_rezervovania_do) ? NULL : $values->datum_rezervovania_do,
			'datum_ubytovania_od' => empty($values->datum_ubytovania_od) ? NULL : $values->datum_ubytovania_od,
			'datum_ubytovania_do' => empty($values->datum_ubytovania_do) ? NULL : $values->datum_ubytovania_do,
			'pocet_osob' =>  $values->pocet_osob,
			'cislo_izby' =>  $values->cislo_izby,
			'id_klienta' => $values->id_klienta,
			'id_zamestnanca' =>$values->id_zamestnanca,
			'id_zlavy' => $values->id_zlavy,
			'id_platby' => $values->id_platby
		], 'WHERE id_pobytu = ?', $this->accomodation->id_pobytu);

		if ($values->cislo_izby != $this->accomodation->cislo_izby) {
			$this->database->query('UPDATE izba SET', [
				'dostupnost' => 1
			], 'WHERE cislo_izby = ?', $this->accomodation->cislo_izby);

			$this->database->query('UPDATE izba SET', [
				'dostupnost' => 0
			], 'WHERE cislo_izby = ?', $values->cislo_izby);
		}

		$this->flashMessage('Pobyt bol upravený v systéme.', 'ok');
	}
	
	/* Formular na pridanie pobytu */
	protected function createComponentAddForm()
    {
		$form = new UI\Form;
		$form->addDate('datum_ubytovania_do', 'Dátum konca ubytovania', DateInput::TYPE_DATE)->setRequired(False)->addRule(UI\Form::VALID);
		$form->addText('pocet_osob', 'Počet osôb (*):')->setRequired()->addRule(UI\Form::INTEGER, 'Počet osôb musí byť číslo');
		
		
		$clients = null;
		foreach ($this->database->query('select id_klienta, meno, priezvisko from klient')->fetchAll() as $row) {
			$clients[$row->id_klienta] = $row->meno . " " . $row->priezvisko;
		}
		$form->addSelect('id_klienta', 'Klient (*):', $clients)->setRequired();

		$discounts = null;
		foreach ($this->database->query('select id_zlavy, nazov from zlava')->fetchAll() as $row) {
			$discounts[$row->id_zlavy] = $row->nazov;
		}
		$form->addSelect('id_zlavy', 'Zľava (*):', $discounts)->setRequired();

		$rooms = null;
		foreach ($this->database->query('select cislo_izby, kapacita from izba where dostupnost = 1')->fetchAll() as $row) {
			$rooms[$row->cislo_izby] = $row->cislo_izby . " (kapacita: " . $row->kapacita . ")";
		}
		$form->addSelect('cislo_izby', 'Izba (*):', $rooms)->setRequired();
		

		$form->addSubmit('add', 'Pridať');
		$form->onSuccess[] = [$this, 'addFormSucceeded'];
        return $form;
	}
	
	/* Akcia po potvrdeni pridania pobytu, vlozenie do db */
	public function addFormSucceeded(UI\Form $form, $values)
    {
			if (!empty($values->datum_ubytovania_od) && !empty($values->datum_ubytovania_do)) {
				$date1 = $values->datum_ubytovania_do;
				$date2 = $values->datum_ubytovania_od;
				$interval = $date2->diff($date1);

				if ($interval->format("%r%a") <= 0) {
					$this->flashMessage('Neplatný dátum konca ubytovania. Koniec ubytovania musí byť neskorší než začiatok ubytovania. Pobyt nebol pridaný do systému.', 'fail');
					return;
				}
			}
		
			$this->database->query('INSERT INTO pobyt', [
				'datum_rezervovania_od' => NULL,
				'datum_rezervovania_do' => NULL,
				'datum_ubytovania_od' => date('Y-m-d H:i:s'),
				'datum_ubytovania_do' => $values->datum_ubytovania_do,
				'pocet_osob' =>  $values->pocet_osob,
				'cislo_izby' =>  $values->cislo_izby,
				'id_klienta' => $values->id_klienta,
				'id_zamestnanca' => $this->getUser()->getId(),
				'id_zlavy' => $values->id_zlavy,
				'id_platby' => $this->database->query('select id_platby, suma from platba where suma = 0')->fetch()->id_platby
			]);

			$this->database->query('UPDATE izba SET', [
				'dostupnost' => 0
			], 'WHERE cislo_izby = ?', $values->cislo_izby);

			$this->flashMessage('Pobyt bol pridaný do systému.', 'ok');
	}
	 
	/* Formular na vytvorenie rezervacie ako host */
	protected function createComponentCreateHostForm()
    {
		$form = new UI\Form;
		$form->addText('meno', 'Meno (*):')->setRequired();
		$form->addText('priezvisko', 'Priezvisko (*):')->setRequired();
		$form->addText('mesto', 'Mesto (*):')->setRequired();
		$form->addText('ulica', 'Ulica:');
		$form->addText('cislo_domu', 'Číslo domu (*):')->setRequired()->addRule(UI\Form::INTEGER, 'Číslo domu musí byť číslo');
		$form->addText('stat', 'Štát (*):')->setRequired();
		$form->addText('psc', 'PSČ (*):')->setRequired()->addRule(UI\Form::INTEGER, 'PSČ musí byť číslo');
		$form->addText('rodne_cislo', 'Rodné číslo (*):')->setRequired()->addRule(UI\Form::INTEGER, 'Rodné číslo musí byť číslo')
		->addRule(UI\Form::MAX_LENGTH, 'Rodné číslo musí mať maximálne %d znakov', 10)
		->addRule(UI\Form::MIN_LENGTH, 'Rodné číslo musí mať mimálne %d znakov', 10);
		$form->addText('telefonne_cislo', 'Telefónne číslo  (+42X):')->setRequired(False)->addRule(UI\Form::MAX_LENGTH, 'Telefónne číslo smie mať maximálne %d znakov', 13);;
		$form->addEmail('email', 'Emailová adresa:');
		$form->addText('login', 'Prihlasovacie meno (*):')->setRequired();
		$form->addPassword('heslo', 'Heslo (*):')->setRequired();

		$form->addDate('datum_rezervovania_od', 'Dátum začiatku rezervácie', DateInput::TYPE_DATE)->setRequired(False)->addRule(UI\Form::VALID);
		$form->addDate('datum_rezervovania_do', 'Dátum konca rezervácie', DateInput::TYPE_DATE)->setRequired(False)->addRule(UI\Form::VALID);
		$form->addText('pocet_osob', 'Počet osôb (*):')->setRequired()->addRule(UI\Form::INTEGER, 'Počet osôb musí byť číslo');
		
		
		$form->addSubmit('add', 'Pridať');
		$form->onSuccess[] = [$this, 'createHostFormSucceeded'];
        return $form;
	}

	/* Akcia po potvrdeni vytvorenia hostovskej rezervacie, vlozenie do db */
	public function createHostFormSucceeded(UI\Form $form, $values)
    {
		if (!empty($values->datum_rezervovania_od) && !empty($values->datum_rezervovania_do)) {
			$date1 = $values->datum_rezervovania_do;
			$date2 = $values->datum_rezervovania_od;
			$interval = $date2->diff($date1);

			if ($interval->format("%r%a") <= 0) {
				$this->flashMessage('Neplatný dátum konca rezervácie. Koniec rezervácie musí byť neskorší než začiatok rezervácie. Rezervácia nebola pridaná do systému.', 'fail');
				return;
			}
		}

		$user = $this->database->query('select id_klienta from klient where login = ?', $values->login)->fetchAll();;
		if (count($user) == 0) {
			$this->database->query('INSERT INTO klient', [
				'meno' => $values->meno,
				'priezvisko' => $values->priezvisko,
				'mesto' =>  $values->mesto,
				'ulica' => $values->ulica,
				'cislo_domu' => $values->cislo_domu,
				'stat' => $values->meno,
				'psc' => $values->psc,
				'rodne_cislo' =>  $values->rodne_cislo,
				'telefonne_cislo' => $values->telefonne_cislo,
				'email' => $values->email,
				'login' => $values->login,
				'heslo' => base64_encode($values->heslo)
			]);

			$new_client = $this->database->getInsertId();
		} else {
			$this->flashMessage('Klient s týmto prihlasovacím menom už v systéme existuje. Prosím, zvoľte iné. Rezervácia nebola pridaná do systému.', 'fail');
			return;
		}

		$rezervacna_zlava = $this->database->query('select * from zlava where rezervacna_zlava = 1 and sezonna_zlava = 0')->fetch()->id_zlavy;
		
		$system = $this->database->query('select * from zamestnanec natural join spravca')->fetch()->id_zamestnanca;
        	$this->database->query('INSERT INTO pobyt', [
				'datum_rezervovania_od' => $values->datum_rezervovania_od,
				'datum_rezervovania_do' => $values->datum_rezervovania_do,
				'datum_ubytovania_od' => NULL,
				'datum_ubytovania_do' => NULL,
				'pocet_osob' =>  $values->pocet_osob,
				'id_klienta' => $new_client,
				'id_zamestnanca' => $system,
				'id_zlavy' => $rezervacna_zlava,
				'id_platby' => $this->database->query('select id_platby, suma from platba where suma = 0')->fetch()->id_platby
			]);

			$this->flashMessage('Rezervácia bola pridaná do systému.', 'ok');
	}

	/* Formular na vytvorenie rezervacie ako sucasny klient */
	protected function createComponentCreateForm()
    {
		$form = new UI\Form;
		$form->addDate('datum_rezervovania_od', 'Dátum začiatku rezervácie', DateInput::TYPE_DATE)->setRequired(False)->addRule(UI\Form::VALID);
		$form->addDate('datum_rezervovania_do', 'Dátum konca rezervácie', DateInput::TYPE_DATE)->setRequired(False)->addRule(UI\Form::VALID);
		$form->addText('pocet_osob', 'Počet osôb (*):')->setRequired()->addRule(UI\Form::INTEGER, 'Počet osôb musí byť číslo');
		
		
		if (!$this->getUser()->isInRole('klient')) {
			$clients = null;
			foreach ($this->database->query('select id_klienta, meno, priezvisko from klient')->fetchAll() as $row) {
				$clients[$row->id_klienta] = $row->meno . " " . $row->priezvisko;
			}
			$form->addSelect('id_klienta', 'Klient (*):', $clients)->setRequired();
		}

		$discounts = null;
		foreach ($this->database->query('select id_zlavy, nazov from zlava')->fetchAll() as $row) {
			$discounts[$row->id_zlavy] = $row->nazov;
		}
		$form->addSelect('id_zlavy', 'Zľava (*):', $discounts)->setRequired();

		$form->addSubmit('add', 'Pridať');
		$form->onSuccess[] = [$this, 'createFormSucceeded'];
        return $form;
	}
	
	/* Akcia po potvrdeni vytvorenia klientskej rezervacie, vlozenie do db */
	public function createFormSucceeded(UI\Form $form, $values)
    {
			if (!empty($values->datum_rezervovania_od) && !empty($values->datum_rezervovania_do)) {
				$date1 = $values->datum_rezervovania_do;
				$date2 = $values->datum_rezervovania_od;
				$interval = $date2->diff($date1);

				if ($interval->format("%r%a") <= 0) {
					$this->flashMessage('Neplatný dátum konca rezervácie. Koniec rezervácie musí byť neskorší než začiatok rezervácie. Rezervácia nebola pridaná do systému.', 'fail');
					return;
				}
			}

			$system = $this->database->query('select * from zamestnanec natural join spravca')->fetch()->id_zamestnanca;
        	$this->database->query('INSERT INTO pobyt', [
				'datum_rezervovania_od' => $values->datum_rezervovania_od,
				'datum_rezervovania_do' => $values->datum_rezervovania_do,
				'datum_ubytovania_od' => NULL,
				'datum_ubytovania_do' => NULL,
				'pocet_osob' =>  $values->pocet_osob,
				'id_klienta' => empty($values->id_klienta) ? $this->getUser()->getId() : $values->id_klienta,
				'id_zamestnanca' => $this->getUser()->isInRole('klient') ? $system : $this->getUser()->getId(),
				'id_zlavy' => $values->id_zlavy,
				'id_platby' => $this->database->query('select id_platby, suma from platba where suma = 0')->fetch()->id_platby
			]);

			$this->flashMessage('Rezervácia bola pridaná do systému.', 'ok');
	}
	
	/* Formular na pridanie sluzby k pobytu */
	protected function createComponentAddServiceForm()
    {
		$form = new UI\Form;
		$services = null;
		foreach ($this->database->query('select id_sluzby, nazov from sluzba')->fetchAll() as $row) {
			$found = False;
			foreach ($this->database->query('select id_pobytu, id_sluzby, nazov from sluzba_pobyt natural join sluzba where id_pobytu = ?', $this->id_pobytu)->fetchAll() as $service) {
				if ($row->id_sluzby == $service->id_sluzby) {
					$found = True;
				}
			}

			if ($found == False) {
				$services[$row->id_sluzby] = $row->nazov;
			}
		}
		$form->addSelect('id_sluzby', 'Služba (*):', $services)->setRequired();

		$form->addSubmit('add', 'Pridať');
		$form->onSuccess[] = [$this, 'addServiceFormSucceeded'];
        return $form;
	}
	
	/* Akcia po potvrdeni pridanie sluzby k pobytu, vlozenie do db */
	public function addServiceFormSucceeded(UI\Form $form, $values)
    {
        	$this->database->query('INSERT INTO sluzba_pobyt', [
				'id_sluzby' => $values->id_sluzby,
				'id_pobytu' => $this->id_pobytu
			]);

			$this->flashMessage('Služba bola pridaná k pobytu.', 'ok');
	}

	/* Formular na odstranenie sluzby pre pobyt */
	protected function createComponentDeleteServiceForm()
    {
		$form = new UI\Form;
		$services = null;
		foreach ($this->database->query('select id_pobytu, id_sluzby, nazov from sluzba_pobyt natural join sluzba where id_pobytu = ?', $this->id_pobytu)->fetchAll() as $row) {
			$services[$row->id_sluzby] = $row->nazov;
		}
		$form->addSelect('id_sluzby', 'Služba (*):', $services)->setRequired();

		$form->addSubmit('delete', 'Vymazať');
		$form->onSuccess[] = [$this, 'deleteServiceFormSucceeded'];
        return $form;
	}
	
	/* Akcia po potvrdeni odstranenia sluzby pre pobyt, odstranenie z db */
	public function deleteServiceFormSucceeded(UI\Form $form, $values)
    {
        	$this->database->query('DELETE FROM sluzba_pobyt WHERE id_sluzby = ? and id_pobytu = ?', $values->id_sluzby, $this->id_pobytu);

			$this->flashMessage('Služba bola odstránená z pobytu.', 'ok');
	}

	public function actionShowServices($id) {
		foreach ($this->database->query('select id_pobytu, id_sluzby, nazov from sluzba_pobyt natural join sluzba where id_pobytu = ?', $id)->fetchAll() as $row) {
			$this->flashMessage($row->nazov, 'ok');
		}
	}
	
	/* Formular pre zaplatenie pobytu */
	protected function createComponentPayForm()
    {
		$form = new UI\Form;
		if ($this->accomodation->cislo_izby == NULL || $this->accomodation->datum_ubytovania_od == NULL || $this->accomodation->datum_ubytovania_do == NULL) {
			return $form;
		}		
		$form->addText('suma', 'Suma v eurách (*):')->setRequired()->addRule(UI\Form::INTEGER, 'Cena musí byť číslo');
		$form->addSelect('sposob_platby', 'Spôsob platby (*):', [
			'1' => 'prevodom z účtu',
			'2' => 'v hotovosti'
		])->setRequired();
		$form->addText('cislo_uctu', 'Číslo účtu v IBAN:')->setRequired(False)
		->addRule(UI\Form::MAX_LENGTH, 'Číslo účtu musí mať maximálne %d znakov', 24)
		->addRule(UI\Form::MIN_LENGTH, 'Číslo účtu musí mať mimálne %d znakov', 24)
		->addRule(UI\Form::PATTERN, 'Nesprávny formát čísla účtu', '[A-Z]{2}[0-9]{1,32}');

		$date1 = new DateTime($this->accomodation->datum_ubytovania_do);
		$date2 = new DateTime($this->accomodation->datum_ubytovania_od);
		$interval = $date1->diff($date2);
		$suma = $interval->d; // dni

		$room = $this->database->query('select cena from izba where cislo_izby = ?', $this->accomodation->cislo_izby)->fetch();
		$suma = $suma * $room->cena;
		$this->flashMessage('Počet dní: ' . $interval->d, 'listok');
		$this->flashMessage('Cena za noc: ' . $room->cena . ' eur', 'listok');

		foreach ($this->database->query('select id_sluzby from sluzba_pobyt where id_pobytu = ?', $this->id_pobytu)->fetchAll() as $row) {
			$service = $this->database->query('select id_sluzby, nazov, cena from sluzba where id_sluzby = ?', $row->id_sluzby)->fetch();
			$suma += $service->cena;
			$this->flashMessage($service->nazov . ': ' . $service->cena . ' eur', 'listok');
		}

		$discount = $this->database->query('select * from zlava where id_zlavy = ?', $this->accomodation->id_zlavy)->fetch();
		
		if ($discount->rezervacna_zlava == 1) {
			$suma = $suma * 0.9;
			$this->flashMessage('Rezervačná zľava: 10%', 'listok');
		} 
		
		if ($discount->sezonna_zlava == 1) {
			$suma = $suma * 0.95;
			$this->flashMessage('Sezónna zľava: 5%', 'listok');
		} 

		$form->addSubmit('confirm', 'Pridať platbu');
		$form->onSuccess[] = [$this, 'payFormSucceeded'];
		
		$form->setDefaults([
			'suma' => $suma
		]);
        return $form;
    }
	
	/* Akcia po potvrdeni zadania platby, vlozenie do db */
    public function payFormSucceeded(UI\Form $form, $values)
    {
		$this->database->query('INSERT INTO platba', [
			'suma' => $values->suma,
			'datum_platby' => date('Y-m-d H:i:s'),
			'sposob_platby' =>  $values->sposob_platby,
			'cislo_uctu' => $values->cislo_uctu,
			'id_klienta' => $this->accomodation->id_klienta,
			'id_zamestnanca' => $this->getUser()->getId()
		]);

        $this->database->query('UPDATE pobyt SET', [
			'id_platby' => $this->database->getInsertId(),
			'id_zamestnanca' => $this->getUser()->getId()
		], 'WHERE id_pobytu = ?', $this->id_pobytu);

		$this->database->query('UPDATE izba SET', [
		    'dostupnost' => 1
		], 'WHERE cislo_izby = ?', $this->accomodation->cislo_izby);

		$this->flashMessage('Platba za pobyt bola zaevidovaná v systéme.', 'ok');
	}
	
	/* Formular na zacatie pobytu */
	protected function createComponentAccomodateForm()
    {
		$form = new UI\Form;
		$form->addDate('datum_ubytovania_do', 'Dátum konca ubytovania', DateInput::TYPE_DATE)->setRequired(False)->addRule(UI\Form::VALID);
		$form->addText('pocet_osob', 'Počet osôb (*):')->setRequired()->addRule(UI\Form::INTEGER, 'Počet osôb musí byť číslo');

		$discounts = null;
		foreach ($this->database->query('select id_zlavy, nazov from zlava')->fetchAll() as $row) {
			$discounts[$row->id_zlavy] = $row->nazov;
		}
		$form->addSelect('id_zlavy', 'Zľava (*):', $discounts)->setRequired();

		$rooms = null;
		foreach ($this->database->query('select cislo_izby, kapacita from izba where dostupnost = 1')->fetchAll() as $row) {
			$rooms[$row->cislo_izby] = $row->cislo_izby . " (kapacita: " . $row->kapacita . ")";
		}
		$form->addSelect('cislo_izby', 'Izba (*):', $rooms)->setRequired();
		

		$form->addSubmit('add', 'Pridať');
		$form->onSuccess[] = [$this, 'accomodateFormSucceeded'];
        return $form;
	}
	
	/* Akcia po potvrdeni zacatia pobytu, vlozenie do db */
	public function accomodateFormSucceeded(UI\Form $form, $values)
    {
			if (!empty($values->datum_ubytovania_od) && !empty($values->datum_ubytovania_do)) {
				$date1 = $values->datum_ubytovania_do;
				$date2 = $values->datum_ubytovania_od;
				$interval = $date2->diff($date1);

				if ($interval->format("%r%a") <= 0) {
					$this->flashMessage('Neplatný dátum konca ubytovania. Koniec ubytovania musí byť neskorší než začiatok ubytovania. Pobyt nebol pridaný do systému.', 'fail');
					return;
				}
			}
		
			$this->database->query('UPDATE pobyt SET', [
				'datum_rezervovania_od' => NULL,
				'datum_rezervovania_do' => NULL,
				'datum_ubytovania_od' => date('Y-m-d H:i:s'),
				'datum_ubytovania_do' => $values->datum_ubytovania_do,
				'pocet_osob' =>  $values->pocet_osob,
				'cislo_izby' =>  $values->cislo_izby,
				'id_klienta' => $this->accomodation->id_klienta,
				'id_zamestnanca' => $this->getUser()->getId(),
				'id_zlavy' => $values->id_zlavy,
				'id_platby' => $this->database->query('select id_platby, suma from platba where suma = 0')->fetch()->id_platby
			], 'WHERE id_pobytu = ?', $this->accomodation->id_pobytu);

			$this->database->query('UPDATE izba SET', [
				'dostupnost' => 0
			], 'WHERE cislo_izby = ?', $values->cislo_izby);

			$this->flashMessage('Pobyt bol pridaný do systému.', 'ok');
	}
}	
