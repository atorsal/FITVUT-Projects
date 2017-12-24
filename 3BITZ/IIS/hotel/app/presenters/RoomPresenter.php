<?php

namespace App\Presenters;

use Nette;
use Nette\Application\UI;
use Nette\Database\ForeignKeyConstraintViolationException;
use Nette\Database\UniqueConstraintViolationException;
use \DateTime;


/**
* Sprava izieb
*/
class RoomPresenter extends BasePresenter
{
	private $database;
	private $room;

    public function __construct(Nette\Database\Context $database)
    {
        $this->database = $database;
    }

	/* Odstranenie izby */
	public function actionDelete($id) {
		$this->room = $this->database->table("izba")->where('cislo_izby', $id)->fetch();
		if (!$this->room) {
			$this->redirect("add");
		}

		try {
			$this->room->delete();
			$this->flashMessage('Izba bola odstránená z ponuky.', 'ok');
		} catch (ForeignKeyConstraintViolationException $ex) {
			$this->flashMessage('Izba je sučasťou pobytu.', 'fail');
		}
	}

	/* Uprava izby */
	public function actionEdit($id) {
		$this->room =  $this->database->table("izba")->where('cislo_izby', $id)->fetch();
		if (!$this->room) {
			$this->redirect("add");
		}
	}

	/* Zobrazenie pobytov na izbe */
	public function actionView($id) {
		$pobyty = $this->database->query('select * from pobyt')->fetchAll();
		foreach ($pobyty as $row) {
			if ($row->cislo_izby == $id) {
				$from = (new DateTime($row->datum_ubytovania_od))->format('d. m. Y');
				$to = (new DateTime($row->datum_ubytovania_do))->format('d. m. Y');
				$this->flashMessage('Pobyt od ' . $from  .' do ' . $to, 'ok');
			}
		}
	}

	/* Formular na pridanie izby */
	protected function createComponentAddForm()
    {
        $form = new UI\Form;
		$form->addText('cislo_izby', 'Cislo izby (*):')->setRequired()->addRule(UI\Form::INTEGER, 'Cena musí byť číslo');
		$form->addText('kapacita', 'Kapacita (*):')->setRequired()->addRule(UI\Form::INTEGER, 'Kapacita musí byť číslo');
		$form->addText('pocet_miestnosti', 'Počet miestností (*):')->setRequired()->addRule(UI\Form::INTEGER, 'Počet miestností musí byť číslo');
		$form->addSelect('dostupnost', 'Dostupnosť (*):', [
			'0' => 'nie',
			'1' => 'áno'
		])->setRequired();
		
		$form->addText('cena', 'Cena za deň v eurách (*):')->setRequired()->addRule(UI\Form::INTEGER, 'Cena musí byť číslo');
		$form->addSubmit('add', 'Pridať');
        $form->onSuccess[] = [$this, 'addFormSucceeded'];
        return $form;
    }

	/* Akcia po potvrdeni pridania izby, vlozenie do db */
    public function addFormSucceeded(UI\Form $form, $values)
    {
        try {
			$this->database->query('INSERT INTO izba', [
			'cislo_izby' => $values->cislo_izby,
			'kapacita' => $values->kapacita,
			'pocet_miestnosti' =>  $values->pocet_miestnosti,
			'dostupnost' => $values->dostupnost,
			'cena' => $values->cena
			]);

			$this->flashMessage('Izba bola pridaná do ponuky.', 'ok');
		
		} catch (UniqueConstraintViolationException $ex) {
			$this->flashMessage('Izba s týmto číslom už existuje v ponuke.', 'fail');
		}
	}
	
	/* Formular na upravu izby */
	protected function createComponentEditForm()
    {
		$form = new UI\Form;
		$form->addText('cislo_izby', 'Cislo izby (*):')->setRequired()->addRule(UI\Form::INTEGER, 'Cena musí byť číslo');
		$form->addText('kapacita', 'Kapacita (*):')->setRequired()->addRule(UI\Form::INTEGER, 'Kapacita musí byť číslo');
		$form->addText('pocet_miestnosti', 'Počet miestností (*):')->setRequired()->addRule(UI\Form::INTEGER, 'Počet miestností musí byť číslo');
		$form->addSelect('dostupnost', 'Dostupnosť (*):', [
			'0' => 'nie',
			'1' => 'áno'
		])->setRequired();
		
		$form->addText('cena', 'Cena za deň v eurách (*):')->setRequired()->addRule(UI\Form::INTEGER, 'Cena musí byť číslo');
        $form->addSubmit('confirm', 'Potvrdiť zmeny');
		$form->onSuccess[] = [$this, 'editFormSucceeded'];
		
		$form->setDefaults([
			'cislo_izby' => $this->room->cislo_izby,
			'kapacita' =>  $this->room->kapacita,
			'pocet_miestnosti' =>  $this->room->pocet_miestnosti,
			'dostupnost' =>  $this->room->dostupnost,
			'cena' =>  $this->room->cena
		]);
        return $form;
    }

	/* Akcia po potvrdeni zmien u izby, aktualizacia db */
    public function editFormSucceeded(UI\Form $form, $values)
    {
        try {
			$this->database->query('UPDATE izba SET', [
			'cislo_izby' => $values->cislo_izby,
			'kapacita' => $values->kapacita,
			'pocet_miestnosti' =>  $values->pocet_miestnosti,
			'dostupnost' => $values->dostupnost,
			'cena' => $values->cena
		], 'WHERE cislo_izby = ?', $this->room->cislo_izby);

			$this->flashMessage('Izba bola upravená v ponuke.', 'ok');
		} catch (ForeignKeyConstraintViolationException $ex) {
			$this->flashMessage('Izba je sučasťou aktuálne prebiehajúceho pobytu.', 'fail');
		}
    }
}	
