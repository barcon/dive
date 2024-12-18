#ifndef DIVE_STATUS_HPP_
#define DIVE_STATUS_HPP_

#include "dive_types.hpp"
#include <map>

namespace dive
{

static const Status DIVE_SUCCESS			=  0;
static const Status DIVE_NOT_FOUND			= -1;
static const Status DIVE_NULL_POINTER		= -2;
static const Status DIVE_ALREADY_EXISTS		= -3;
static const Status DIVE_INVALID_PROBLEM	= -4;
static const Status DIVE_INVALID_ELEMENT	= -5;
static const Status DIVE_INVALID_TYPE		= -6;
static const Status DIVE_INVALID_FORMAT		= -7;
static const Status DIVE_OUT_OF_RANGE		= -8;
static const Status DIVE_MAX_OF_RANGE		= -9;
static const Status DIVE_BUFFER_OVERFLOW	= -10;
static const Status DIVE_STILL_IN_USE		= -11;
static const Status DIVE_THREAD_RUNNING		= -12;
static const Status DIVE_THREAD_PAUSED		= -13;
static const Status DIVE_THREAD_STOPPED		= -14;
static const Status DIVE_THREAD_FINISHED	= -15;
static const Status DIVE_FILE_NOT_OPENED	= -16;
static const Status DIVE_UNDEFINED			= -99;

static const std::map<Status, String> messages =
{
	{  0, "DIVE_SUCCESS"},
	{ -1, "DIVE_NOT_FOUND"},
	{ -2, "DIVE_NULL_POINTER"},
	{ -3, "DIVE_ALREADY_EXISTS"},
	{ -4, "DIVE_INVALID_PROBLEM"},
	{ -5, "DIVE_INVALID_ELEMENT"},
	{ -6, "DIVE_INVALID_TYPE"},
	{ -7, "DIVE_INVALID_FORMAT"},
	{ -8, "DIVE_OUT_OF_RANGE"},
	{ -9, "DIVE_MAX_OF_RANGE"},
	{-10, "DIVE_BUFFER_OVERFLOW"},
	{-11, "DIVE_STILL_IN_USE"},
	{-12, "DIVE_THREAD_RUNNING"},
	{-13, "DIVE_THREAD_PAUSED"},
	{-14, "DIVE_THREAD_STOPPED"},
	{-15, "DIVE_THREAD_FINISHED"},
	{-16, "DIVE_FILE_NOT_OPENED"},
	{-99, "DIVE_UNDEFINED"}
};

}

#endif