<?php

namespace App\Presenters;

use Nette;
use Nette\Application\UI;

/**
* Hlavna stranka
*/
class MainpagePresenter extends BasePresenter
{
	private $database;

    public function __construct(Nette\Database\Context $database)
    {
        $this->database = $database;
    }
	/* Zobrazenie uvodne stranky */
	public function renderDefault()
	{
			
	}
	/* Zobrazenie sluzieb */
	public function renderService() {
		$this->template->services = $this->database->table('sluzba');
	}
	/* Zobrazenie zliav */
	public function renderDiscount() {
		$this->template->discounts = $this->database->table('zlava');
	}
	/* Zobrazenie izieb */
	public function renderRoom() {
		$this->template->rooms = $this->database->table('izba');
	}
	/* Zobrazenie klientov */
	public function renderClient() {
		$this->template->clients = $this->database->table('klient');
	}
	/* Zobrazenie zamestnancov */
	public function renderEmployee() {
		$this->template->employees = $this->database->table('zamestnanec');
	}
	/* Zobrazenie platieb */
	public function renderPayment() {
		$this->template->payments = $this->database->query('select platba.*, klient.meno as meno_klienta, klient.priezvisko as priezvisko_klienta, zamestnanec.meno as meno_zamestnanca, zamestnanec.priezvisko as priezvisko_zamestnanca from platba join klient join zamestnanec where platba.id_klienta = klient.id_klienta and platba.id_zamestnanca = zamestnanec.id_zamestnanca')->fetchAll();
	}
	/* Zobrazenie pobytov/rezervacii */
	public function renderAccomodation() {
		if ($this->getUser()->isInRole('klient')) {
			$this->template->accomodations = $this->database->query('select pobyt.*, klient.meno as meno_klienta, klient.priezvisko as priezvisko_klienta, zamestnanec.meno as meno_zamestnanca, zamestnanec.priezvisko as priezvisko_zamestnanca, zlava.nazov as nazov_zlavy, platba.suma as suma_platby from pobyt join klient join platba join zamestnanec join zlava where pobyt.id_klienta = klient.id_klienta and pobyt.id_zamestnanca = zamestnanec.id_zamestnanca and pobyt.id_zlavy = zlava.id_zlavy and pobyt.id_platby = platba.id_platby and pobyt.id_klienta = ?', $this->getUser()->getId())->fetchAll();
		} else {
			$this->template->accomodations = $this->database->query('select pobyt.*, klient.meno as meno_klienta, klient.priezvisko as priezvisko_klienta, zamestnanec.meno as meno_zamestnanca, zamestnanec.priezvisko as priezvisko_zamestnanca, zlava.nazov as nazov_zlavy, platba.suma as suma_platby from pobyt join klient join platba join zamestnanec join zlava where pobyt.id_klienta = klient.id_klienta and pobyt.id_zamestnanca = zamestnanec.id_zamestnanca and pobyt.id_zlavy = zlava.id_zlavy and pobyt.id_platby = platba.id_platby and pobyt.id_klienta = klient.id_klienta')->fetchAll();
		}
	}
    /* Prechod na domovsku stranku */
    public function actionHome()
	{
		$this->redirect('Homepage:');
	}
}	
