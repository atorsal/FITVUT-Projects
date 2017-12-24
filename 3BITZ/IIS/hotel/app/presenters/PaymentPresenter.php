<?php

namespace App\Presenters;

use Nette;
use Nette\Application\UI;
use Nette\Database\ForeignKeyConstraintViolationException;
use Nette\Database\UniqueConstraintViolationException;
use Vodacek\Forms\Controls\DateInput;

/**
* Sprava platieb 
*/
class PaymentPresenter extends BasePresenter
{
	private $database;
	private $payment;

    public function __construct(Nette\Database\Context $database)
    {
        $this->database = $database;
    }

	/* Uprava platby */
	public function actionEdit($id) {
		$this->payment =  $this->database->table("platba")->where('id_platby', $id)->fetch();
		if (!$this->payment) {
			$this->redirect("add");
		}
	}

	/* Formular na upravu platby */
	protected function createComponentEditForm()
    {
		$form = new UI\Form;
		$form->addText('suma', 'Suma v eurách (*):')->setRequired()->addRule(UI\Form::INTEGER, 'Cena musí byť číslo');
		$form->addDate('datum_platby', 'Dátum platby', DateInput::TYPE_DATE)->setRequired()->addRule(UI\Form::VALID);
		$form->addSelect('sposob_platby', 'Spôsob platby (*):', [
			'1' => 'prevodom z účtu',
			'2' => 'v hotovosti'
		])->setRequired();
		$form->addText('cislo_uctu', 'Číslo účtu v IBAN:')->setRequired(False)
		->addRule(UI\Form::MAX_LENGTH, 'Číslo účtu musí mať maximálne %d znakov', 24)
		->addRule(UI\Form::MIN_LENGTH, 'Číslo účtu musí mať mimálne %d znakov', 24)
		->addRule(UI\Form::PATTERN, 'Nesprávny formát čísla účtu', '[A-Z]{2}+[0-9]{1,32}');

		$clients = null;
		foreach ($this->database->query('select id_klienta, meno, priezvisko from klient')->fetchAll() as $row) {
			$clients[$row->id_klienta] = $row->meno . " " . $row->priezvisko;
		}
		$form->addSelect('id_klienta', 'Zaplatil/a (*):', $clients)->setRequired();

		$employees = null;
		foreach ($this->database->query('select id_zamestnanca, meno, priezvisko from zamestnanec')->fetchAll() as $row) {
			$employees[$row->id_zamestnanca] = $row->meno . " " . $row->priezvisko;
		}
		$form->addSelect('id_zamestnanca', 'Zaevidoval/a (*):', $employees)->setRequired();
		
		
		$form->addSubmit('confirm', 'Potvrdiť zmeny');
		$form->onSuccess[] = [$this, 'editFormSucceeded'];
		
		$form->setDefaults([
			'suma' => $this->payment->suma,
			'datum_platby' => $this->payment->datum_platby,
			'sposob_platby' =>  $this->payment->sposob_platby,
			'cislo_uctu' =>  $this->payment->cislo_uctu,
			'id_klienta' => $this->payment->id_klienta,
			'id_zamestnanca' =>$this->payment->id_zamestnanca
		]);
        return $form;
    }

	/* Akcia po potvrdeni zmien u platby */
    public function editFormSucceeded(UI\Form $form, $values)
    {
        $this->database->query('UPDATE platba SET', [
			'suma' => $values->suma,
			'datum_platby' => $values->datum_platby,
			'sposob_platby' =>  $values->sposob_platby,
			'cislo_uctu' => $values->cislo_uctu,
			'id_klienta' => $values->id_klienta,
			'id_zamestnanca' => $values->id_zamestnanca
		], 'WHERE id_platby = ?', $this->payment->id_platby);

		$this->flashMessage('Platba bola upravená v systéme.', 'ok');
    }
}	
