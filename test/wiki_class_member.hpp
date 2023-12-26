// HFSM2 (hierarchical state machine for games and interactive applications)
// Created by Andrew Gresyk

#pragma once

#include <cstddef> // max_align_t

////////////////////////////////////////////////////////////////////////////////

class Actor {
public:
	struct alignas(std::max_align_t) FsmHost {
		char buffer[104]; // the size is hand-adjusted
	};

public:
	Actor();
	~Actor();

	void turnOn();

	bool isOn() const;

private:
	FsmHost _fsmHost;
};

////////////////////////////////////////////////////////////////////////////////
