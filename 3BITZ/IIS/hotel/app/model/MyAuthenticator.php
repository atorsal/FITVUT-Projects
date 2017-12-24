<?php

namespace App\Model;
use Nette\Security as NS;

class MyAuthenticator  extends \Nette\Object implements NS\IAuthenticator
{
    public $database;

    function __construct(\Nette\Database\Context $database)
    {
        $this->database = $database;
    }

    function authenticate(array $credentials)
    {
        list($username, $password) = $credentials;
        $row = $this->database->table('klient')
            ->where('login', $username)->fetch();
		
		$isClient = False;
        if (!$row) {
			$row = $this->database->table('zamestnanec')->where('login', $username)->fetch();
            if (!$row) {
				throw new NS\AuthenticationException('Účet nenájdený. Prosím, skontrolujte prihlasovacie meno.');
        	}
			$admin = $this->database->query('select * from zamestnanec natural join spravca where login = ?', $username)->fetch();
			$role = $admin ? "spravca" : "zamestnanec";
        } else {
			$role = "klient";
		}
		

        if (base64_decode($row->heslo) !== $password) {
            throw new NS\AuthenticationException('Zadali ste nesprávne heslo.');
        }

		$name = $row->meno . " " . $row->priezvisko;

		if ($role == "klient") {
			return new NS\Identity($row->id_klienta, $role, ['name' => $name]);
		} else {
			return new NS\Identity($row->id_zamestnanca, $role, ['name' => $name]);
		}
	}
}
?>
