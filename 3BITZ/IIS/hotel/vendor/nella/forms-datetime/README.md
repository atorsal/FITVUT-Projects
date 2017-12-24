Date/DateTime control for [Nette Framework](http://nette.org)
=============================================================

[![Build Status](https://img.shields.io/travis/nella/forms-datetime.svg?style=flat-square)](https://travis-ci.org/nella/forms-datetime)
[![Code Coverage](https://img.shields.io/coveralls/nella/forms-datetime.svg?style=flat-square)](https://coveralls.io/r/nella/forms-datetime)
[![SensioLabsInsight Status](https://img.shields.io/sensiolabs/i/26a217ba-02c8-4fd5-a93b-cb58db3e2811.svg?style=flat-square)](https://insight.sensiolabs.com/projects/26a217ba-02c8-4fd5-a93b-cb58db3e2811)
[![Latest Stable Version](https://img.shields.io/packagist/v/nella/forms-datetime.svg?style=flat-square)](https://packagist.org/packages/nella/forms-datetime)
[![Composer Downloads](https://img.shields.io/packagist/dt/nella/forms-datetime.svg?style=flat-square)](https://packagist.org/packages/nella/forms-datetime)
[![Dependency Status](https://img.shields.io/versioneye/d/user/projects/5467a35df8a4aeff6c0000e5.svg?style=flat-square)](https://www.versioneye.com/user/projects/5467a35df8a4aeff6c0000e5)

Requirements
------------
- Nette >=2.3.0 (2.3.x support will be removed on 31 Jan 2017)
- PHP >=5.5.0 (5.5.x support will be removed on 10 Jul 2016)

Installation
------------

```
composer require nella/forms-datetime
```

Usage
------

```php

$form = new \Nette\Forms\Form;
$form->addComponent(new \Nella\Forms\DateTime\DateInput('Date'), 'date');
$form->addComponent(new \Nella\Forms\DateTime\DateTimeInput('DateTime'), 'datetime');

// or

\Nella\Forms\DateTime\DateInput::register();
$form->addDate('date', 'Date', 'Y-m-d');

\Nella\Forms\DateTime\DateTimeInput::register();
$form->addDateTime('datetime', 'DateTime', 'Y-m-d', 'G:i');

// Optional date[time] validation
$form['date']
	->addCondition(\Nette\Application\UI\Form::FILLED)
		->addRule([$form['date'], 'validateDate'], 'Date is invalid');

$form['datetime']
	->addCondition(\Nette\Application\UI\Form::FILLED)
		->addRule([$form['datetime'], 'validateDateTime'], 'Date time is invalid');

```

Manual rendering
----------------

```smarty
{form myForm}
	{label date /}
	{input date}

	{label datetime /}
    {input datetime:date}
    {input datetime:time}
{/form}
```

License
-------
Date/DateTime control for Nette Framework is licensed under the MIT License - see the LICENSE file for details
