<?php
/**
 * This file is part of the Nella Project (http://nella-project.org).
 *
 * Copyright (c) Patrik Votoček (http://patrik.votocek.cz)
 *
 * For the full copyright and license information,
 * please view the file LICENSE.md that was distributed with this source code.
 */

namespace Nella\Forms\DateTime;

use Nette\Forms\Container;
use Nette\Forms\Form;

/**
 * Date input form control
 *
 * @author Patrik Votoček
 *
 * @property string $value
 */
class DateInput extends \Nette\Forms\Controls\BaseControl
{

	const DEFAULT_FORMAT = 'Y-m-d';

	/** @var bool */
	private static $registered = FALSE;

	/** @var string */
	private $format;

	/** @var bool */
	private $strict = FALSE;

	/**
	 * @param string $format
	 * @param string|NULL $label
	 */
	public function __construct($format = self::DEFAULT_FORMAT, $label = NULL)
	{
		parent::__construct($label);
		$this->format = $format;

		$this->getControlPrototype()->data('nella-date-format', $format);
	}

	/**
	 * @return \Nella\Forms\DateTime\DateInput
	 */
	public function enableStrict()
	{
		$this->strict = TRUE;
		return $this;
	}

	/**
	 * @return \Nella\Forms\DateTime\DateInput
	 */
	public function disableStrict()
	{
		$this->strict = FALSE;
		return $this;
	}

	/**
	 * @param \DateTimeInterface|NULL $value
	 * @return \Nella\Forms\DateTime\DateInput
	 */
	public function setValue($value = NULL)
	{
		if ($value === NULL) {
			return parent::setValue(NULL);
		} elseif (!$value instanceof \DateTimeInterface) {
			throw new \Nette\InvalidArgumentException('Value must be DateTimeInterface or NULL');
		}

		return parent::setValue($value->format($this->format));
	}

	/**
	 * @return \DateTimeImmutable|NULL
	 */
	public function getValue()
	{
		if (!$this->isFilled()) {
			return NULL;
		}

		$datetime = \DateTimeImmutable::createFromFormat($this->format, $this->getRawValue());
		if ($datetime === FALSE || $datetime->format($this->format) !== $this->getRawValue()) {
			return NULL;
		}

		return $datetime->setTime(0, 0, 0);
	}

	/**
	 * @return mixed
	 */
	public function getRawValue()
	{
		return parent::getValue();
	}

	public function loadHttpData()
	{
		$input = $this->getHttpData(\Nette\Forms\Form::DATA_TEXT);
		if (empty($input)) {
			parent::setValue(NULL);
			return;
		}

		$datetime = \DateTimeImmutable::createFromFormat(
			$this->normalizeFormat($this->format),
			$this->normalizeFormat($input)
		);

		if ($datetime !== FALSE
			&& $datetime->format($this->normalizeFormat($this->format)) === $this->normalizeFormat($input)
		) {
			parent::setValue($datetime->format($this->format));
			return;
		}

		parent::setValue('');
	}

	/**
	 * @return \Nette\Utils\Html
	 */
	public function getControl()
	{
		$control = parent::getControl();
		$control->value($this->getRawValue());
		$control->type('text');
		return $control;
	}

	/**
	 * @return bool
	 */
	public function isFilled()
	{
		$value = $this->getRawValue();
		return $value !== NULL;
	}

	/**
	 * @return bool
	 */
	public function validateDate()
	{
		return $this->isDisabled() || !$this->isFilled() || $this->getValue() !== NULL;
	}

	/**
	 * @param string $input
	 * @return string
	 */
	private function normalizeFormat($input)
	{
		if ($this->strict) {
			return $input;
		}

		return \Nette\Utils\Strings::replace($input, '~\s+~', '');
	}

	/**
	 * @param string|bool $message
	 * @return \Nella\Forms\DateTime\DateInput
	 */
	public function setRequired($message = TRUE)
	{
		if ($message !== FALSE && !is_string($message)) {
			throw new \Nette\InvalidArgumentException('Message must be string');
		}

		parent::setRequired($message);

		if ($message !== FALSE) {
			$this->addCondition(Form::FILLED)
				->addRule(function (DateInput $control) {
					return $this->validateDate($control);
				}, $message);
		}

		return $this;
	}

	public static function register()
	{
		if (static::$registered) {
			throw new \Nette\InvalidStateException('DateInput control already registered.');
		}

		static::$registered = TRUE;

		$class = get_called_class();
		$callback = function (
			Container $container,
			$name,
			$label = NULL,
			$format = self::DEFAULT_FORMAT
		) use ($class) {
			$control = new $class($format, $label);
			$container->addComponent($control, $name);
			return $control;
		};

		Container::extensionMethod('addDate', $callback);
	}

}
