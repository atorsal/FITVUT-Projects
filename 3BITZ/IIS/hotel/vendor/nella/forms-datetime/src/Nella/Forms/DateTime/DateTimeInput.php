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

use DateTimeImmutable;
use Nette\Forms\Container;
use Nette\Forms\Form;

/**
 * Date time input form control
 *
 * @author Patrik Votoček
 *
 * @property string $value
 */
class DateTimeInput extends \Nette\Forms\Controls\BaseControl
{

	const DEFAULT_DATE_FORMAT = 'Y-m-d';
	const DEFAULT_TIME_FORMAT = 'G:i';

	const FORMAT_PATTERN = '%s %s';

	const NAME_DATE = 'date';
	const NAME_TIME = 'time';

	/** @var bool */
	private static $registered = FALSE;

	/** @var string */
	private $dateFormat;

	/** @var string */
	private $timeFormat;

	/** @var string */
	private $date;

	/** @var string */
	private $time;

	/** @var mixed[]|array */
	private $dateAttributes = [];

	/** @var mixed[]|array */
	private $timeAttributes = [];

	/** @var bool */
	private $sanitizeShortHour = TRUE;

	/** @var bool */
	private $strict = FALSE;

	/** @var \DateTimeImmutable|null */
	private $defaultTime;

	/**
	 * @param string $dateFormat
	 * @param string $timeFormat
	 * @param string|NULL $label
	 */
	public function __construct(
		$dateFormat = self::DEFAULT_DATE_FORMAT,
		$timeFormat = self::DEFAULT_TIME_FORMAT,
		$label = NULL
	)
	{
		parent::__construct($label);
		$this->dateFormat = $dateFormat;
		$this->timeFormat = $timeFormat;
	}

	/**
	 * @return \Nella\Forms\DateTime\DateTimeInput
	 */
	public function enableStrict()
	{
		$this->strict = TRUE;
		return $this;
	}

	/**
	 * @return \Nella\Forms\DateTime\DateTimeInput
	 */
	public function disableStrict()
	{
		$this->strict = FALSE;
		return $this;
	}

	/**
	 * @param \DateTimeInterface|NULL $value
	 * @return \Nella\Forms\DateTime\DateTimeInput
	 */
	public function setValue($value = NULL)
	{
		if ($value === NULL) {
			$this->date = NULL;
			$this->time = NULL;
			return $this;
		} elseif (!$value instanceof \DateTimeInterface) {
			throw new \Nette\InvalidArgumentException('Value must be DateTimeInterface or NULL');
		}

		$this->date = $value->format($this->dateFormat);
		$this->time = $value->format($this->timeFormat);

		return $this;
	}

	/**
	 * @return \DateTimeImmutable|NULL
	 */
	public function getValue()
	{
		if ($this->date === NULL || $this->time === NULL) {
			return NULL;
		}

		$format = sprintf(static::FORMAT_PATTERN, $this->dateFormat, $this->timeFormat);
		$datetimeString = sprintf(static::FORMAT_PATTERN, $this->date, $this->time);

		$datetime = DateTimeImmutable::createFromFormat($format, $datetimeString);

		if ($datetime === FALSE || $datetime->format($format) !== $datetimeString) {
			return NULL;
		}

		return $datetime;
	}

	/**
	 * @return bool
	 */
	public function isFilled()
	{
		return $this->date !== NULL || $this->time !== NULL;
	}

	public function loadHttpData()
	{
		$this->date = $this->getHttpData(Form::DATA_LINE, '[' . static::NAME_DATE . ']');
		$this->time = $this->getHttpData(Form::DATA_LINE, '[' . static::NAME_TIME . ']');

		if (empty($this->date)) {
			$this->date = NULL;
		}
		if (empty($this->time)) {
			$this->time = NULL;
		}
		if (empty($this->date) && empty($this->time)) {
			return;
		}

		if ($this->sanitizeShortHour && \Nette\Utils\Strings::startsWith(\Nette\Utils\Strings::lower($this->timeFormat), 'g')) {
			if (\Nette\Utils\Strings::startsWith($this->time, '00')) {
				$this->time = \Nette\Utils\Strings::substring($this->time, 1);
			}
		}

		$inputString = sprintf(
			static::FORMAT_PATTERN,
			$this->normalizeFormat($this->date),
			$this->normalizeFormat($this->time)
		);
		$datetimeFormat = sprintf(
			static::FORMAT_PATTERN,
			$this->normalizeFormat($this->dateFormat),
			$this->normalizeFormat($this->timeFormat)
		);
		$datetime = \DateTimeImmutable::createFromFormat($datetimeFormat, $inputString);

		if ($datetime === FALSE || $datetime->format($datetimeFormat) !== $inputString) {
			$this->date = '';
			$this->time = '';
			return;
		}

		$this->date = $datetime->format($this->dateFormat);
		$this->time = $datetime->format($this->timeFormat);
	}

	/**
	 * @return string
	 */
	public function getControl()
	{
		return $this->getControlPart(static::NAME_DATE) . $this->getControlPart(static::NAME_TIME);
	}

	/**
	 * @param string|NULL $key
	 * @return \Nette\Utils\Html
	 */
	public function getControlPart($key = NULL)
	{
		$name = $this->getHtmlName();

		if ($key === NULL) {
			return $this->getControl();
		} elseif ($key === static::NAME_DATE) {
			$control = \Nette\Utils\Html::el('input')->name($name . '[' . static::NAME_DATE . ']');
			$control->data('nella-date-format', $this->dateFormat);
			$control->value($this->date);
			$control->type('text');

			$control->disabled($this->disabled);

			$control->addAttributes($this->dateAttributes);

			return $control;
		} elseif ($key === static::NAME_TIME) {
			$control = \Nette\Utils\Html::el('input')->name($name . '[' . static::NAME_TIME . ']');
			$control->data('nella-time-format', $this->timeFormat);
			$control->value(
				$this->time === NULL && $this->defaultTime !== NULL ? $this->defaultTime->format($this->timeFormat) : $this->time
			);
			$control->type('text');

			$control->disabled($this->disabled);

			$control->addAttributes($this->timeAttributes);

			return $control;
		}

		throw new \Nette\InvalidArgumentException('Part ' . $key . ' does not exist');
	}

	public function getLabelPart()
	{
		return NULL;
	}

	/**
	 * @return bool
	 */
	public function validateDateTime()
	{
		return $this->isDisabled() || !$this->isFilled() || $this->getValue() !== NULL;
	}

	/**
	 * @param string $name
	 * @param mixed $value
	 * @return \Nella\Forms\DateTime\DateTimeInput
	 */
	public function setDateAttribute($name, $value = TRUE)
	{
		if ($value === NULL) {
			unset($this->dateAttributes[$name]);
		} else {
			$this->dateAttributes[$name] = $value;
		}

		return $this;
	}

	/**
	 * @param string $name
	 * @param mixed $value
	 * @return \Nella\Forms\DateTime\DateTimeInput
	 */
	public function setTimeAttribute($name, $value = TRUE)
	{
		if ($value === NULL) {
			unset($this->timeAttributes[$name]);
		} else {
			$this->timeAttributes[$name] = $value;
		}

		return $this;
	}

	/**
	 * @param string $name
	 * @param mixed $value
	 * @return \Nella\Forms\DateTime\DateTimeInput
	 */
	public function setAttribute($name, $value = TRUE)
	{
		$this->setDateAttribute($name, $value);
		$this->setTimeAttribute($name, $value);

		return $this;
	}

	public function disableShortHourSanitizer()
	{
		$this->sanitizeShortHour = FALSE;
	}

	public function setDefaultTime(\DateTimeImmutable $defaultTime = NULL)
	{
		$this->defaultTime = $defaultTime;
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
	 * @return \Nella\Forms\DateTime\DateTimeInput
	 */
	public function setRequired($message = TRUE)
	{
		if ($message !== FALSE && !is_string($message)) {
			throw new \Nette\InvalidArgumentException('Message must be string');
		}

		parent::setRequired($message);

		if ($message !== FALSE) {
			$this->addCondition(Form::FILLED)
				->addRule(function (DateTimeInput $control) {
					return $this->validateDateTime($control);
				}, $message);
		}

		return $this;
	}

	public static function register()
	{
		if (static::$registered) {
			throw new \Nette\InvalidStateException('DateTimeInput control already registered.');
		}

		static::$registered = TRUE;

		$class = get_called_class();
		$callback = function (
			Container $container,
			$name,
			$label = NULL,
			$dateFormat = self::DEFAULT_DATE_FORMAT,
			$timeFormat = self::DEFAULT_TIME_FORMAT
		) use ($class) {
			$control = new $class($dateFormat, $timeFormat, $label);
			$container->addComponent($control, $name);
			return $control;
		};

		Container::extensionMethod('addDateTime', $callback);
	}

}
