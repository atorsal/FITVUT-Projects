<?php

namespace App\Presenters;

use Nette;
use Nette\Application\UI;
use Nette\Database\ForeignKeyConstraintViolationException;
use Nette\Database\UniqueConstraintViolationException;


/**
* Sprava klientov
*/
class ClientPresenter extends BasePresenter
{
	private $database;
	private $client;

    public function __construct(Nette\Database\Context $database)
    {
        $this->database = $database;
    }

	/* Odstranenie klienta */
	public function actionDelete($id) {
		$this->client = $this->database->table("klient")->where('id_klienta', $id)->fetch();
		if (!$this->client) {
			$this->redirect("add");
		}

		try {
			$this->client->delete();
			$this->flashMessage('Klient bol odstránený zo systému.', 'ok');
		} catch (ForeignKeyConstraintViolationException $ex) {
			$this->flashMessage('Klient je súčasťou pobytu.', 'fail');
		}
	}

	/* Uprava udajov o klientovi */
	public function actionEdit($id) {
		$this->client =  $this->database->table("klient")->where('id_klienta', $id)->fetch();
		if (!$this->client) {
			$this->redirect("add");
		}
	}

	/* Formular na pridanie klienta */
	protected function createComponentAddForm()
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
		->addRule(UI\Form::MIN_LENGTH, 'Rodné číslo musí mať minimálne %d znakov', 10);
		$form->addText('telefonne_cislo', 'Telefónne číslo  (+42X):')->setRequired(False)->addRule(UI\Form::MAX_LENGTH, 'Telefónne číslo smie mať maximálne %d znakov', 13);;
		$form->addEmail('email', 'Emailová adresa:');
		$form->addText('login', 'Prihlasovacie meno (*):')->setRequired();
		$form->addPassword('heslo', 'Heslo (*):')->setRequired();
		
		$form->addSubmit('add', 'Pridať');
        $form->onSuccess[] = [$this, 'addFormSucceeded'];
        return $form;
    }

	/* Akcia po potvrdeni pridania klienta, vlozenie do db */
    public function addFormSucceeded(UI\Form $form, $values)
    {
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

			$this->flashMessage('Klient bol pridaný do systému.', 'ok');
	}
	
	/* Formular na upravu udajov o klientovi */
	protected function createComponentEditForm()
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
		->addRule(UI\Form::MIN_LENGTH, 'Rodné číslo musí mať minimálne %d znakov', 10);
		$form->addText('telefonne_cislo', 'Telefónne číslo  (+42X):')->setRequired(False)->addRule(UI\Form::MAX_LENGTH, 'Telefónne číslo smie mať maximálne %d znakov', 13);;
		$form->addEmail('email', 'Emailová adresa:');
		$form->addText('login', 'Prihlasovacie meno (*):')->setRequired();
		$form->addText('heslo', 'Heslo (*):')->setRequired();
        $form->addSubmit('confirm', 'Potvrdiť zmeny');
		$form->onSuccess[] = [$this, 'editFormSucceeded'];
		
		$form->setDefaults([
			'meno' => $this->client->meno,
			'priezvisko' => $this->client->priezvisko,
			'mesto' =>  $this->client->mesto,
			'ulica' => $this->client->ulica,
			'cislo_domu' => $this->client->cislo_domu,
			'stat' => $this->client->meno,
			'psc' => $this->client->psc,
			'rodne_cislo' =>  $this->client->rodne_cislo,
			'telefonne_cislo' => $this->client->telefonne_cislo,
			'email' => $this->client->email,
			'login' => $this->client->login,
			'heslo' => base64_decode($this->client->heslo)
		]);
        return $form;
    }

	/* Akcia po potvrdeni zmien u klienta, aktualizacia db */
    public function editFormSucceeded(UI\Form $form, $values)
    {
        $this->database->query('UPDATE klient SET', [
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
		], 'WHERE id_klienta = ?', $this->client->id_klienta);

		$this->flashMessage('Klient bol upravený upravený v systéme.', 'ok');
    }
}	
