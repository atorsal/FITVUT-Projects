<?php

namespace App\Presenters;

use Nette;
use Nette\Application\UI;
use \Datetime;
use \Exception;

/**
* Domovska stranka, prihlasovanie do IS 
*/
class HomepagePresenter extends BasePresenter
{
	private $database;

    public function __construct(Nette\Database\Context $database)
    {
        $this->database = $database;
    }
	
	/* Hlavna stranka */
	public function renderDefault()
	{
	}

	/* Informacie o obsadeni hotela */
	public function actionRefresh($date){
		$date = substr($_SERVER['REQUEST_URI'], strrpos($_SERVER['REQUEST_URI'], '/') + 1, 10);
		try {
			$datef = new DateTime($date);
			$this->template->date = $datef->format('d. m. Y');
		} catch (Exception $e) {
			$this->redirect('Homepage:');
		}

		$this->template->dvojizbove_volne = $this->template->dvojizbove_vsetky = count($this->database->table('izba')->where("kapacita", 2));
		$this->template->trojizbove_volne = $this->template->trojizbove_vsetky = count($this->database->table('izba')->where("kapacita", 3));
		$this->template->stvorizbove_volne = $this->template->stvorizbove_vsetky = count($this->database->table('izba')->where("kapacita", 4));
		$pobyty = $this->database->query('select * from pobyt where ? between datum_ubytovania_od and datum_ubytovania_do', $date)->fetchAll();
		
		foreach ($pobyty as $row) {
			 $izba = $this->database->table("izba")->where('cislo_izby', $row->cislo_izby)->fetch();
			 if ($izba->kapacita == 2) {
				$this->template->dvojizbove_volne--;
			 }
			 elseif ($izba->kapacita == 3) {
				$this->template->trojizbove_volne--;
			 }
			 elseif ($izba->kapacita == 4) {
				$this->template->stvorizbove_volne--;
			 }
		}
	}


	/* Formular na prihlasenie do IS */
	protected function createComponentLoginForm()
    {
        $form = new UI\Form;
        $form->addText('username', 'Prihlasovacie meno:')->setRequired();
        $form->addPassword('password', 'Heslo:')->setRequired();
        $form->addSubmit('login', 'Prihlásiť')->setHtmlAttribute('style', 'background-color: #0275d8; color: white; border-radius: .25rem;');
        $form->onSuccess[] = [$this, 'loginFormSucceeded'];
        return $form;
    }

	/* Akcia po prihlaseni do IS */
    public function loginFormSucceeded(UI\Form $form, $values)
    {
        $this->getUser()->setExpiration('20 minutes', TRUE);
		
		try {
			$this->getUser()->login($values->username, $values->password);
			$this->redirect('Mainpage:Accomodation');
		} catch (Nette\Security\AuthenticationException $e) {
			$form->addError($e->getMessage());
		}
    }

	/* Odhlasenie z IS */
	public function actionLogout()
	{
		$this->getUser()->logout();
		$this->redirect('Homepage:');
	}
	
	/* Prechod na stranku spravy pobytov a rezervacii */
	public function actionManage()
	{
		$this->redirect('Mainpage:Accomodation');
	}
}	
