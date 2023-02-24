#pragma once

#include <iostream>
#include <iomanip>
#include <ctime>
#include <thread>
#include <atomic>

using namespace std;

#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define DEFAULT "\033[0m"

// #define DEBUG

inline size_t _readableThreadId() {
    static atomic<size_t> thread_idx{0};
    thread_local size_t id = thread_idx;
    thread_idx++;
    return id;
}

inline void _print_logs(const string message, const char* level) {
#ifdef DEBUG
	auto time = std::time(nullptr);
	stringstream msg;
	msg << "[" << put_time(localtime(&time), "%T") << "][" << level << "] " << _readableThreadId() << ": " << message;
	cout << msg.str(); // thread safe
#endif
}

inline void LOG(const string message) {
	_print_logs(message, GREEN "LOG" DEFAULT);
}

inline void ERROR(const string message) {
	_print_logs(message, RED "ERROR" DEFAULT);
}

inline void WARNING(const string message) {
	_print_logs(message, YELLOW "WARNING" DEFAULT);
}
