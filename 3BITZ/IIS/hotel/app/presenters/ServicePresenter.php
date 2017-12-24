<?php

namespace App\Presenters;

use Nette;
use Nette\Application\UI;
use Nette\Database\ForeignKeyConstraintViolationException;

/**
* Sprava sluzieb
*/
class ServicePresenter extends BasePresenter
{
	private $database;
	private $service;

    public function __construct(Nette\Database\Context $database)
    {
        $this->database = $database;
    }

	/* Odstranenie sluzby */
	public function actionDelete($id) {
		$this->service = $this->database->table("sluzba")->where('id_sluzby', $id)->fetch();
		if (!$this->service) {
			$this->redirect("add");
		}

		try {
			$this->service->delete();
			$this->flashMessage('Služba bola odstránená z ponuky.', 'ok');
		} catch (ForeignKeyConstraintViolationException $ex) {
			$this->flashMessage('Služba je sučasťou pobytu.', 'fail');
		}
	}

	/* Uprava sluzby */
	public function actionEdit($id) {
		$this->service = $this->database->table("sluzba")->where('id_sluzby', $id)->fetch();
		if (!$this->service) {
			$this->redirect("add");
		}
	}

	/* Formular na pridanie sluzby */
	protected function createComponentAddForm()
    {
        $form = new UI\Form;
        $form->addText('nazov', 'Názov (*):')->setRequired();
		$form->addSelect('typ', 'Typ (*):', [
			'1' => 'šesťhodinová',
			'2' => 'dvojhodinová',
			'3' => 'osemhodinová',
			'4' => 'celodenná'
		])->setRequired();
		$form->addText('cena', 'Cena v eurach (*):')->setRequired()->addRule(UI\Form::INTEGER, 'Cena musí byť číslo');
        $form->addSubmit('add', 'Pridať');
        $form->onSuccess[] = [$this, 'addFormSucceeded'];
        return $form;
    }

	/* Akcia po potvrdeni pridania sluzby, vlozenie do db */
    public function addFormSucceeded(UI\Form $form, $values)
    {
        $this->database->query('INSERT INTO sluzba', [
			'nazov' => $values->nazov,
			'typ' => $values->typ,
			'cena' => $values->cena,
		]);

		$this->flashMessage('Služba bola pridaná do ponuky.', 'ok');
	}
	
	/* Formular na upravu sluzby */
	protected function createComponentEditForm()
    {
		$form = new UI\Form;
        $form->addText('nazov', 'Názov:')->setRequired();
        $form->addSelect('typ', 'Typ (*):', [
			'1' => 'šesťhodinová',
			'2' => 'dvojhodinová',
			'3' => 'osemhodinová',
			'4' => 'celodenná'
		])->setRequired();
		$form->addText('cena', 'Cena v eurach (*):')->setRequired()->setRequired()->addRule(UI\Form::INTEGER, 'Cena musí byť číslo');
        $form->addSubmit('confirm', 'Potvrdiť zmeny');
		$form->onSuccess[] = [$this, 'editFormSucceeded'];
		
		$form->setDefaults([
			'nazov' => $this->service->nazov,
			'typ' => $this->service->typ,
			'cena' => $this->service->cena
		]);
        return $form;
    }

	/* Akcia po potvrdeni zmien sluzby, aktualizacia v db */
    public function editFormSucceeded(UI\Form $form, $values)
    {
        $this->database->query('UPDATE sluzba SET', [
			'nazov' => $values->nazov,
			'typ' => $values->typ,
			'cena' => $values->cena,
		], 'WHERE id_sluzby = ?', $this->service->id_sluzby);
		
		$this->flashMessage('Služba bola upravená v ponuke.', 'ok');
    }
}	
