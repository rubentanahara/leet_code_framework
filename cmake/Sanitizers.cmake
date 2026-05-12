add_library(lc_sanitizers INTERFACE)

option(LC_ENABLE_SANITIZERS "Enable AddressSanitizer + UBSan in Debug builds" ON)

if(LC_ENABLE_SANITIZERS)
    set(_san "$<$<CONFIG:Debug>:-fsanitize=address,undefined;-fno-omit-frame-pointer>")
    target_compile_options(lc_sanitizers INTERFACE ${_san})
    target_link_options(lc_sanitizers INTERFACE ${_san})
endif()
