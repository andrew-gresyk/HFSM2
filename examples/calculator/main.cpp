// added by Kjeld Mathias Petersen
// in https://github.com/DonMathi/HFSM2/commit/d258e9c0790cde92e04d98402e6b2636cbaf57fe
// based on https://www.embedded.com/print/4026976

#include <hfsm2/machine.hpp>

#include <iostream>
#include <conio.h>


#define DEBUG 1
#if DEBUG==1
#define PRINTCALL std::cout << __LINE__ << ": " << __FUNCTION__ << std::endl
#else
#define PRINTCALL
#endif


enum MathOperator { IDLE, PLUS, MINUS, MULTIPLY, DIVIDE };
const char* const MathOperatorString[] = { "?", "+", "-", "*", "/" }; // NOLINT(cert-err58-cpp)


struct Context {
	double operand1 = {0.0};
	MathOperator mathOperator = {MathOperator::IDLE};
	double operand2 = {0.0};
	double decimalFactor = {1.0};

	void error()
	{
		PRINTCALL;
		std::cout << ">>>!!! Error !!!<<<" << std::endl;
		display();
	}

	void clearOperand1()
	{
		PRINTCALL;
		operand1 = 0.0;
	}

	void clearOperand2()
	{
		PRINTCALL;
		operand2 = 0.0;
	}

	void reset()
	{
		PRINTCALL;
		clearOperand1();
		clearOperand2();
		mathOperator = MathOperator::IDLE;
		endFraction();
	}

	void display()
	{
		PRINTCALL;
		std::cout << "\n";
		std::cout << "[" << MathOperatorString[mathOperator] << "," << decimalFactor << "]\n";
		std::cout << "2: " << operand2 << "\n";
		std::cout << "1: " << operand1 << std::endl;
	}

	void beginFraction()
	{
		PRINTCALL;
		decimalFactor = 10.0;
	}

	void endFraction()
	{
		PRINTCALL;
		decimalFactor = 1.0;
	}

	void negateOperand1()
	{
		PRINTCALL;
		operand1 = - operand1;
	}

	void negateOperand2()
	{
		PRINTCALL;
		operand2 = - operand2;
	}

	void insertInOperand1(int digit)
	{
		PRINTCALL;
		if (decimalFactor > 1.0)
		{
			operand1 += std::copysign((double)digit / decimalFactor, operand1);
			decimalFactor *= 10.0;
		}
		else
		{
			operand1 *= 10.0;
			operand1 += std::copysign(digit, operand1);
		}
	}

	void insertInOperand2(int digit)
	{
		PRINTCALL;
		if (decimalFactor != 1.0) //-V550
		{
			operand2 += std::copysign(digit / decimalFactor, operand2);
			decimalFactor *= 10.0;
		}
		else
		{
			operand2 *= 10.0;
			operand2 += std::copysign(digit, operand2);
		}
	}

	bool performOp()
	{
		PRINTCALL;
		switch(mathOperator)
		{
			case PLUS:
				operand1 += operand2;
				return true;

			case MINUS:
				operand1 -= operand2;
				return true;

			case MULTIPLY:
				operand1 *= operand2;
				return true;

			case DIVIDE:
				if (operand2 != 0.0) //-V550
				{
					operand1 /= operand2;
					return true;
				}
				else
					return false;

			default:
				return false;
		}
	}
};



// Events
struct TurnOn {};
struct Clear {}; // C
struct TurnOff {};
struct ClearEntry {}; // CE
struct Digit_0 {}; // 0
struct Digit_1_9 { int digit; }; // 1..9
struct Point {}; // .
struct Operator { MathOperator mathOperator; }; // +, -, *, /
struct Equals {}; // =



// FSM Structure
using M = hfsm2::MachineT<hfsm2::Config::ContextT<Context>>;
using FSM = M::PeerRoot<
	struct Off,
	M::Composite<struct On,
		M::Composite<struct Ready,
			struct Begin,
			struct Result
		>,
		struct Negated1,
		M::Composite<struct Operand1,
			struct Zero1,
			struct Int1,
			struct Frac1
		>,
		struct Error,
		struct opEntered,
		struct Negated2,
		M::Composite<struct Operand2,
			struct Zero2,
			struct Int2,
			struct Frac2
		>
	>
>;



// Off
struct Off : FSM::State
{
	using FSM::State::react;

	void react(const TurnOn&, FullControl& control)
	{
		PRINTCALL;
		control.changeTo<On>();
	}
};



// On
struct On : FSM::State
{
	using FSM::State::react;

	void enter(Control& control)
	{
		PRINTCALL;
		control.context().reset();
	}

	void reenter(Control& control)
	{
		PRINTCALL;
		control.context().reset();
	}

	void react(const Clear&, FullControl& control)
	{
		PRINTCALL;
		control.changeTo<On>();
	}

	void react(const TurnOff&, FullControl& control)
	{
		PRINTCALL;
		control.changeTo<Off>();
	}
};



// On::Ready
struct Ready : FSM::State
{
	using FSM::State::react;

	void react(const Digit_0&, FullControl& control)
	{
		PRINTCALL;
		control.context().clearOperand1();
		control.context().insertInOperand1(0);
		control.changeTo<Zero1>();
	}

	void react(const Digit_1_9& digit, FullControl& control)
	{
		PRINTCALL;
		control.context().clearOperand1();
		control.context().insertInOperand1(digit.digit);
		control.changeTo<Int1>();
	}

	void react(const Point&, FullControl& control)
	{
		PRINTCALL;
		control.context().clearOperand1();
		control.changeTo<Frac1>();
	}

	void react(const Operator& op, FullControl& control)
	{
		PRINTCALL;
		// This is where bottom-op invocation order is needed
		// In this higher level state I have to ask, if a lower level is active or not
		if (control.isActive<Begin>() && op.mathOperator==MathOperator::MINUS)
			control.changeTo<Negated1>();
		else
		{
			control.context().mathOperator = op.mathOperator;
			control.changeTo<opEntered>();
		}
	}
};



// On::Ready::Begin
struct Begin : FSM::State
{
	using FSM::State::react;

	void enter(Control& control)
	{
		PRINTCALL;
		control.context().clearOperand1();
		control.context().display();
	}
};



// On::Ready::Result
struct Result : FSM::State
{
	using FSM::State::react;

	void enter(Control& control)
	{
		PRINTCALL;
		control.context().display();
	}

	void reenter(Control& control)
	{
		PRINTCALL;
		control.context().display();
	}

	void react(const Equals&, FullControl& control)
	{
		PRINTCALL;
		control.context().performOp();
		control.changeTo<Result>();
	}
};



// On::Negated1
struct Negated1 : FSM::State
{
	using FSM::State::react;

	void enter(Control& control)
	{
		PRINTCALL;
		control.context().negateOperand1();
		control.context().display();
	}

	void reenter(Control& control)
	{
		PRINTCALL;
		control.context().negateOperand1();
		control.context().display();
	}

	void react(const ClearEntry&, FullControl& control)
	{
		PRINTCALL;
		control.changeTo<Begin>();
	}

	void react(const Digit_0&, FullControl& control)
	{
		PRINTCALL;
		control.changeTo<Zero1>();
	}

	void react(const Digit_1_9& digit, FullControl& control)
	{
		PRINTCALL;
		control.context().insertInOperand1(digit.digit);
		control.changeTo<Int1>();
	}

	void react(const Point&, FullControl& control)
	{
		PRINTCALL;
		control.changeTo<Frac1>();
	}

	void react(const Operator& op, FullControl& control)
	{
		PRINTCALL;
		if (op.mathOperator == MathOperator::MINUS)
			control.changeTo<Negated1>();
	}
};



// On::Operand1
struct Operand1 : FSM::State
{
	using FSM::State::react;

	void react(const ClearEntry&, FullControl& control)
	{
		PRINTCALL;
		control.context().clearOperand1();
		control.changeTo<Ready>();
	}

	void react(const Operator& op, FullControl& control)
	{
		PRINTCALL;
		control.context().mathOperator = op.mathOperator;
		control.changeTo<opEntered>();
	}
};



// On::Operand1::Zero1
struct Zero1 : FSM::State
{
	using FSM::State::react;

	void enter(Control& control)
	{
		PRINTCALL;
		control.context().insertInOperand1(0);
		control.context().display();
	}

	void reenter(Control& control)
	{
		PRINTCALL;
		control.context().insertInOperand1(0);
		control.context().display();
	}

	void react(const Digit_0&, FullControl& control)
	{
		PRINTCALL;
		control.changeTo<Zero1>();
	}

	void react(const Digit_1_9& digit, FullControl& control)
	{
		PRINTCALL;
		control.context().insertInOperand1(digit.digit);
		control.changeTo<Int1>();
	}

	void react(const Point&, FullControl& control)
	{
		PRINTCALL;
		control.changeTo<Frac1>();
	}
};



// On::Operand1::Int1
struct Int1 : FSM::State
{
	using FSM::State::react;

	void enter(Control& control)
	{
		PRINTCALL;
		control.context().display();
	}

	void reenter(Control& control)
	{
		PRINTCALL;
		control.context().display();
	}

	void react(const Digit_0&, FullControl& control)
	{
		PRINTCALL;
		control.context().insertInOperand1(0);
		control.changeTo<Int1>();
	}

	void react(const Digit_1_9& digit, FullControl& control)
	{
		PRINTCALL;
		control.context().insertInOperand1(digit.digit);
		control.changeTo<Int1>();
	}

	void react(const Point&, FullControl& control)
	{
		PRINTCALL;
		control.changeTo<Frac1>();
	}
};



// On::Operand1::Frac1
struct Frac1 : FSM::State
{
	using FSM::State::react;

	void enter(Control& control)
	{
		PRINTCALL;
		control.context().beginFraction();
	}

	void reenter(Control& control)
	{
		PRINTCALL;
		control.context().display();
	}

	void exit(Control& control)
	{
		PRINTCALL;
		control.context().endFraction();
	}

	void react(const Digit_0&, FullControl& control)
	{
		PRINTCALL;
		control.context().insertInOperand1(0);
		control.changeTo<Frac1>();
	}

	void react(const Digit_1_9& digit, FullControl& control)
	{
		PRINTCALL;
		control.context().insertInOperand1(digit.digit);
		control.changeTo<Frac1>();
	}
};



// On::Error
struct Error : FSM::State
{
	using FSM::State::react;

	void enter(Control& control)
	{
		PRINTCALL;
		control.context().error();
	}

	void reenter(Control& control)
	{
		PRINTCALL;
		control.context().error();
	}
};



// On::opEntered
struct opEntered : FSM::State
{
	using FSM::State::react;

	void enter(Control& control)
	{
		PRINTCALL;
		control.context().clearOperand2();
		control.context().display();
	}

	void reenter(Control& control)
	{
		PRINTCALL;
		control.context().clearOperand2();
		control.context().display();
	}

	void react(const Digit_0&, FullControl& control)
	{
		PRINTCALL;
		control.context().insertInOperand2(0);
		control.changeTo<Zero2>();
	}

	void react(const Digit_1_9& digit, FullControl& control)
	{
		PRINTCALL;
		control.context().insertInOperand2(digit.digit);
		control.changeTo<Int2>();
	}

	void react(const Point&, FullControl& control)
	{
		PRINTCALL;
		control.changeTo<Frac2>();
	}

	void react(const Operator& op, FullControl& control)
	{
		PRINTCALL;
		if (op.mathOperator==MathOperator::MINUS)
			control.changeTo<Negated2>();
		else
		{
			control.context().mathOperator = op.mathOperator;
			control.changeTo<opEntered>();
		}
	}
};



// On::Negated2
struct Negated2 : FSM::State
{
	using FSM::State::react;

	void enter(Control& control)
	{
		PRINTCALL;
		control.context().negateOperand2();
		control.context().display();
	}

	void reenter(Control& control)
	{
		PRINTCALL;
		control.context().negateOperand2();
		control.context().display();
	}

	void react(const ClearEntry&, FullControl& control)
	{
		PRINTCALL;
		control.changeTo<opEntered>();
	}

	void react(const Digit_0&, FullControl& control)
	{
		PRINTCALL;
		control.changeTo<Zero2>();
	}

	void react(const Digit_1_9& digit, FullControl& control)
	{
		PRINTCALL;
		control.context().insertInOperand2(digit.digit);
		control.changeTo<Int2>();
	}

	void react(const Point&, FullControl& control)
	{
		PRINTCALL;
		control.changeTo<Frac2>();
	}

	void react(const Operator& op, FullControl& control)
	{
		PRINTCALL;
		if (op.mathOperator == MathOperator::MINUS)
			control.changeTo<Negated2>();
	}
};



// On::Operand2
struct Operand2 : FSM::State
{
	using FSM::State::react;

	void enter(Control& control)
	{
		PRINTCALL;
		control.context().display();
	}

	void reenter(Control& control)
	{
		PRINTCALL;
		control.context().display();
	}

	void react(const Operator& op, FullControl& control)
	{
		PRINTCALL;
		if (control.context().performOp())
		{
			control.context().mathOperator = op.mathOperator;
			control.changeTo<opEntered>();
		}
		else
			control.changeTo<Error>();
	}

	void react(const Equals&, FullControl& control)
	{
		PRINTCALL;
		if (control.context().performOp())
			control.changeTo<Result>();
		else
			control.changeTo<Error>();
	}

	void react(const ClearEntry&, FullControl& control)
	{
		PRINTCALL;
		control.context().clearOperand2();
		control.changeTo<opEntered>();
	}
};



// On::Operand2::Zero2
struct Zero2 : FSM::State
{
	using FSM::State::react;

	void enter(Control& control)
	{
		PRINTCALL;
		control.context().insertInOperand2(0);
		control.context().display();
	}

	void reenter(Control& control)
	{
		PRINTCALL;
		control.context().insertInOperand2(0);
		control.context().display();
	}

	void react(const Digit_0&, FullControl& control)
	{
		PRINTCALL;
		control.changeTo<Zero2>();
	}

	void react(const Digit_1_9& digit, FullControl& control)
	{
		PRINTCALL;
		control.context().insertInOperand2(digit.digit);
		control.changeTo<Int2>();
	}

	void react(const Point&, FullControl& control)
	{
		PRINTCALL;
		control.changeTo<Frac2>();
	}
};



// On::Operand2::Int2
struct Int2 : FSM::State
{
	using FSM::State::react;

	void enter(Control& control)
	{
		PRINTCALL;
		control.context().display();
	}

	void reenter(Control& control)
	{
		PRINTCALL;
		control.context().display();
	}

	void react(const Digit_0&, FullControl& control)
	{
		PRINTCALL;
		control.context().insertInOperand2(0);
		control.changeTo<Int2>();
	}

	void react(const Digit_1_9& digit, FullControl& control)
	{
		PRINTCALL;
		control.context().insertInOperand2(digit.digit);
		control.changeTo<Int2>();
	}

	void react(const Point&, FullControl& control)
	{
		PRINTCALL;
		control.changeTo<Frac2>();
	}
};



// On::Operand2::Frac2
struct Frac2 : FSM::State
{
	using FSM::State::react;

	void enter(Control& control)
	{
		PRINTCALL;
		control.context().display();
		control.context().beginFraction();
	}

	void reenter(Control& control)
	{
		PRINTCALL;
		control.context().display();
	}

	void exit(Control& control)
	{
		PRINTCALL;
		control.context().endFraction();
	}

	void react(const Digit_0&, FullControl& control)
	{
		PRINTCALL;
		control.context().insertInOperand2(0);
		control.changeTo<Frac2>();
	}

	void react(const Digit_1_9& digit, FullControl& control)
	{
		PRINTCALL;
		control.context().insertInOperand2(digit.digit);
		control.changeTo<Frac2>();
	}
};



int main()
{
	Context context;
	FSM::Instance machine(context);
	machine.react(TurnOn{});
	int ch;
	do
	{
		PRINTCALL;
		ch = _getch();
		switch(ch)
		{
			case '0': { machine.react(Digit_0{}); break; }
			case '1': { machine.react(Digit_1_9{1}); break; }
			case '2': { machine.react(Digit_1_9{2}); break; }
			case '3': { machine.react(Digit_1_9{3}); break; }
			case '4': { machine.react(Digit_1_9{4}); break; }
			case '5': { machine.react(Digit_1_9{5}); break; }
			case '6': { machine.react(Digit_1_9{6}); break; }
			case '7': { machine.react(Digit_1_9{7}); break; }
			case '8': { machine.react(Digit_1_9{8}); break; }
			case '9': { machine.react(Digit_1_9{9}); break; }
			case 'c': { machine.react(Clear{}); break; }
			case 'e': { machine.react(ClearEntry{}); break; }
			case '+': { machine.react(Operator{MathOperator::PLUS}); break; }
			case '-': { machine.react(Operator{MathOperator::MINUS}); break; }
			case '*': { machine.react(Operator{MathOperator::MULTIPLY}); break; }
			case '/': { machine.react(Operator{MathOperator::DIVIDE}); break; }
			case '.': { machine.react(Point{}); break; } //-V1037
			case ',': { machine.react(Point{}); break; }
			case '=': { machine.react(Equals{}); break; }
			case 'x' : break;
			default:
			{
				ch = ' ';
				break;
			}
		}
	}
	while( ch != 'x' );
	machine.react(TurnOff{});
	return 0;
}
