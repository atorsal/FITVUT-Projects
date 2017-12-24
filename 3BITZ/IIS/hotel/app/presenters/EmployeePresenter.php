<?php

namespace App\Presenters;

use Nette;
use Nette\Application\UI;
use Nette\Database\ForeignKeyConstraintViolationException;
use Nette\Database\UniqueConstraintViolationException;


/**
* Sprava zamestnancov 
*/
class EmployeePresenter extends BasePresenter
{
	private $database;
	private $employee;

    public function __construct(Nette\Database\Context $database)
    {
        $this->database = $database;
    }

	/* Odstranenie zamestnanca */
	public function actionDelete($id) {
		$this->employee = $this->database->table("zamestnanec")->where('id_zamestnanca', $id)->fetch();
		if (!$this->employee) {
			$this->redirect("add");
		}

		try {
			$this->employee->delete();
			$this->flashMessage('Zamestnanec bol odstránený zo systému.', 'ok');
		} catch (ForeignKeyConstraintViolationException $ex) {
			$this->flashMessage('Zamestnanec je súčasťou pobytu.', 'fail');
		}
	}

	/* Uprava udajov o zamestnancovi */
	public function actionEdit($id) {
		$this->employee =  $this->database->table("zamestnanec")->where('id_zamestnanca', $id)->fetch();
		if (!$this->employee) {
			$this->redirect("add");
		}
	}

	/* Formular na pridanie zamestnanca */
	protected function createComponentAddForm()
    {
        $form = new UI\Form;
		$form->addText('meno', 'Meno (*):')->setRequired();
		$form->addText('priezvisko', 'Priezvisko (*):')->setRequired();
		$form->addText('titul', 'Titul:');
		$form->addText('mesto', 'Mesto (*):')->setRequired();
		$form->addText('ulica', 'Ulica:');
		$form->addText('cislo_domu', 'Číslo domu (*):')->setRequired()->addRule(UI\Form::INTEGER, 'Číslo domu musí byť číslo');
		$form->addText('psc', 'PSČ (*):')->setRequired()->addRule(UI\Form::INTEGER, 'PSČ musí byť číslo');
		$form->addText('rodne_cislo', 'Rodné číslo (*):')->setRequired()->addRule(UI\Form::INTEGER, 'Rodné číslo musí byť číslo')
		->addRule(UI\Form::MAX_LENGTH, 'Rodné číslo musí mať maximálne %d znakov', 10)
		->addRule(UI\Form::MIN_LENGTH, 'Rodné číslo musí mať minimálne %d znakov', 10);
		$form->addText('telefonne_cislo', 'Telefónne číslo  (+42X):')->setRequired(False)->addRule(UI\Form::MAX_LENGTH, 'Telefónne číslo smie mať maximálne %d znakov', 13);;
		$form->addEmail('email', 'Emailová adresa:');
		$form->addText('cislo_uctu', 'Číslo účtu v IBAN (*):')->setRequired()
		->addRule(UI\Form::MAX_LENGTH, 'Číslo účtu musí mať maximálne %d znakov', 24)
		->addRule(UI\Form::MIN_LENGTH, 'Číslo účtu musí mať minimálne %d znakov', 24)
		->addRule(UI\Form::PATTERN, 'Nesprávny formát čísla účtu', '[A-Z]{2}[0-9]{1,32}');
		$form->addText('login', 'Prihlasovacie meno (*):')->setRequired();
		$form->addText('heslo', 'Heslo (*):')->setRequired();
		$form->addCheckbox('spravca', 'Správca');
		
		$form->addSubmit('add', 'Pridať');
        $form->onSuccess[] = [$this, 'addFormSucceeded'];
        return $form;
    }

	/* Akcia po potvrdeni pridania zamestnanca, vlozenie do db */
    public function addFormSucceeded(UI\Form $form, $values)
    {
        	$this->database->query('INSERT INTO zamestnanec', [
				'meno' => $values->meno,
				'priezvisko' => $values->priezvisko,
				'titul' =>  $values->titul,
				'mesto' =>  $values->mesto,
				'ulica' => $values->ulica,
				'cislo_domu' => $values->cislo_domu,
				'psc' => $values->psc,
				'rodne_cislo' =>  $values->rodne_cislo,
				'telefonne_cislo' => $values->telefonne_cislo,
				'email' => $values->email,
				'cislo_uctu' => $values->cislo_uctu,
				'login' => $values->login,
				'heslo' => base64_encode($values->heslo)
			]);

			if ($values->spravca) {
				$this->database->query('INSERT INTO spravca', [
					'id_zamestnanca' => $this->database->getInsertId()
				]);
			}

			$this->flashMessage('Zamestnanec bol pridaný do systému.', 'ok');
	}
	
	/* Formular na upravu udajov o zamestnancovi */
	protected function createComponentEditForm()
    {
		$form = new UI\Form;
		$form->addText('meno', 'Meno (*):')->setRequired();
		$form->addText('priezvisko', 'Priezvisko (*):')->setRequired();
		$form->addText('titul', 'Titul:');
		$form->addText('mesto', 'Mesto (*):')->setRequired();
		$form->addText('ulica', 'Ulica:');
		$form->addText('cislo_domu', 'Číslo domu (*):')->setRequired()->addRule(UI\Form::INTEGER, 'Číslo domu musí byť číslo');
		$form->addText('psc', 'PSČ (*):')->setRequired()->addRule(UI\Form::INTEGER, 'PSČ musí byť číslo');
		$form->addText('rodne_cislo', 'Rodné číslo (*):')->setRequired()->addRule(UI\Form::INTEGER, 'Rodné číslo musí byť číslo')
		->addRule(UI\Form::MAX_LENGTH, 'Rodné číslo musí mať maximálne %d znakov', 10)
		->addRule(UI\Form::MIN_LENGTH, 'Rodné číslo musí mať minimálne %d znakov', 10);
		$form->addText('telefonne_cislo', 'Telefónne číslo  (+42X):')->setRequired(False)->addRule(UI\Form::MAX_LENGTH, 'Telefónne číslo smie mať maximálne %d znakov', 13);;
		$form->addEmail('email', 'Emailová adresa:');
		$form->addText('cislo_uctu', 'Číslo účtu v IBAN (*):')->setRequired()
		->addRule(UI\Form::MAX_LENGTH, 'Číslo účtu musí mať maximálne %d znakov', 24)
		->addRule(UI\Form::MIN_LENGTH, 'Číslo účtu musí mať minimálne %d znakov', 24)
		->addRule(UI\Form::PATTERN, 'Nesprávny formát čísla účtu', '[A-Z]{2}[0-9]{1,32}');
		$form->addText('login', 'Prihlasovacie meno (*):')->setRequired();
		$form->addText('heslo', 'Heslo (*):')->setRequired();
		$form->addCheckbox('spravca', 'Správca');
        $form->addSubmit('confirm', 'Potvrdiť zmeny');
		$form->onSuccess[] = [$this, 'editFormSucceeded'];

		$form->setDefaults([
			'meno' => $this->employee->meno,
			'priezvisko' => $this->employee->priezvisko,
			'titul' =>  $this->employee->titul,
			'mesto' =>  $this->employee->mesto,
			'ulica' => $this->employee->ulica,
			'cislo_domu' => $this->employee->cislo_domu,
			'stat' => $this->employee->meno,
			'psc' => $this->employee->psc,
			'rodne_cislo' =>  $this->employee->rodne_cislo,
			'telefonne_cislo' => $this->employee->telefonne_cislo,
			'email' => $this->employee->email,
			'cislo_uctu' => $this->employee->cislo_uctu,
			'login' => $this->employee->login,
			'heslo' => base64_decode($this->employee->heslo),
			'spravca' =>  $this->database->table("spravca")->where('id_zamestnanca', $this->employee->id_zamestnanca)->fetch() != NULL ? True : False
		]);

        return $form;
    }

	/* Akcia po potvrdeni zmien u zamestnaca, aktualizacia db */
    public function editFormSucceeded(UI\Form $form, $values)
    {
        $this->database->query('UPDATE zamestnanec SET', [
			'meno' => $values->meno,
			'priezvisko' => $values->priezvisko,
			'titul' =>  $values->titul,
			'mesto' =>  $values->mesto,
			'ulica' => $values->ulica,
			'cislo_domu' => $values->cislo_domu,
			'psc' => $values->psc,
			'rodne_cislo' =>  $values->rodne_cislo,
			'telefonne_cislo' => $values->telefonne_cislo,
			'email' => $values->email,
			'cislo_uctu' => $values->cislo_uctu,
			'login' => $values->login,
			'heslo' => base64_encode($values->heslo)
		], 'WHERE id_zamestnanca = ?', $this->employee->id_zamestnanca);

		if ($values->spravca) {
			$this->database->query('INSERT INTO spravca', [
				'id_zamestnanca' => $this->employee->id_zamestnanca
			]);
		} else {
			$this->database->query('DELETE FROM spravca WHERE id_zamestnanca = ?', $this->employee->id_zamestnanca);
		}

		$this->flashMessage('Klient bol upravený upravený v systéme.', 'ok');
    }
}	
