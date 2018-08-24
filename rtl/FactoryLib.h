#pragma once
#include "../iface/DeviceIface.h"
#include "Dynamic_Library.h"

namespace factory {
	namespace internal {

		constexpr bool import_test_fails = false;

		void* resolve_factory_symbol(const char* symbol_name);
		void* resolve_not_impl_symbol(const char* symbol_name);
	}

	template <typename T>
	T resolve_symbol(const char* symbol_name) {
		if (internal::import_test_fails) 
			return reinterpret_cast<T>(internal::resolve_not_impl_symbol(symbol_name));

		void *resolution = internal::resolve_factory_symbol(symbol_name);
		if (!resolution) resolution = internal::resolve_not_impl_symbol(symbol_name);	//still returning not_impl if cannot load the symbol

		return reinterpret_cast<T>(resolution);
	}
}
