<?php

namespace App\Presenters;

use Nette;
use Nette\Application\UI;
use Nette\Database\ForeignKeyConstraintViolationException;


class DiscountPresenter extends BasePresenter
{
	private $database;
	private $discount;

    public function __construct(Nette\Database\Context $database)
    {
        $this->database = $database;
    }

	/* Odstranenie zlavy */
	public function actionDelete($id) {
		$this->discount = $this->database->table("zlava")->where('id_zlavy', $id)->fetch();
		if (!$this->discount) {
			$this->redirect("add");
		}

		try {
			$this->discount->delete();
			$this->flashMessage('Zľava bola odstránená z ponuky.', 'ok');
		} catch (ForeignKeyConstraintViolationException $ex) {
			$this->flashMessage('Zľava je sučasťou pobytu.', 'fail');
		}
	}

	/* Uprava zlavy */
	public function actionEdit($id) {
		$this->discount = $this->database->table("zlava")->where('id_zlavy', $id)->fetch();
		if (!$this->discount) {
			$this->redirect("add");
		}
	}

	/* Formular na pridanie zlavy */
	protected function createComponentAddForm()
    {
		$form = new UI\Form;
		$form->addText('nazov', 'Názov (*):')->setRequired();
        $form->addSelect('rezervacna', 'Rezervačná (*):', [
			'0' => 'nie',
			'1' => 'áno',
		])->setRequired();
		$form->addSelect('sezonna', 'Sezónna (*):', [
			'0' => 'nie',
			'1' => 'áno',
		])->setRequired();
		
		$form->addSubmit('add', 'Pridať');
        $form->onSuccess[] = [$this, 'addFormSucceeded'];
        return $form;
    }

	/* Akcia po potvrdeni pridania zlavy, vlozenie do db */
    public function addFormSucceeded(UI\Form $form, $values)
    {
        $this->database->query('INSERT INTO zlava', [
			'nazov' => $values->nazov,
			'rezervacna_zlava' => $values->rezervacna,
			'sezonna_zlava' => $values->sezonna
		]);

		$this->flashMessage('Zľava bola pridaná do ponuky.', 'ok');
	}
	
	/* Formular na upravu zlavy */
	protected function createComponentEditForm()
    {
		$form = new UI\Form;
		$form->addText('nazov', 'Názov (*):')->setRequired();
        $form->addSelect('rezervacna', 'Rezervačná (*):', [
			'0' => 'nie',
			'1' => 'áno',
		])->setRequired();
		$form->addSelect('sezonna', 'Sezónna (*):', [
			'0' => 'nie',
			'1' => 'áno',
		])->setRequired();
		
		$form->addSubmit('confirm', 'Potvrdiť zmeny');
		$form->onSuccess[] = [$this, 'editFormSucceeded'];
		
		$form->setDefaults([
			'nazov' => $this->discount->nazov,
			'rezervacna' => $this->discount->rezervacna_zlava,
			'sezonna' => $this->discount->sezonna_zlava
		]);
        return $form;
    }

	/* Akcia po potvrdeni zmien u zlavy, aktualizacia db */
    public function editFormSucceeded(UI\Form $form, $values)
    {
        $this->database->query('UPDATE zlava SET', [
			'nazov' => $values->nazov,
			'rezervacna_zlava' => $values->rezervacna,
			'sezonna_zlava' => $values->sezonna
		], 'WHERE id_zlavy = ?', $this->discount->id_zlavy);
		
		$this->flashMessage('Zľava bola upravená v ponuke.', 'ok');
    }
}	
