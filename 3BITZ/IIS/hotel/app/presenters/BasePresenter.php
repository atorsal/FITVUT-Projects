<?php

namespace App\Presenters;

use Nette;
use Nette\Application\UI;

abstract class BasePresenter extends \Nette\Application\UI\Presenter{
    public function beforeRender(){
        parent::beforeRender();
		$this->template->loggedIn = $this->getUser()->isLoggedIn();
		$user = $this->getUser();
		if($user->isLoggedIn()){
			$this->template->name = $user->getIdentity()->name;
			if ($user->isInRole('spravca')) {
				$this->template->role = "správca";
			} else if ($user->isInRole('zamestnanec')) {
				$this->template->role = "zamestnanec";
			} else {
				$this->template->role = "klient";
			}
		} else {
			$this->template->name = "neprihlásený";
		}
    }
}
