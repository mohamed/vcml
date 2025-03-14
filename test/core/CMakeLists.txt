 ##############################################################################
 #                                                                            #
 # Copyright 2020 Jan Henrik Weinstock                                        #
 #                                                                            #
 # Licensed under the Apache License, Version 2.0 (the "License");            #
 # you may not use this file except in compliance with the License.           #
 # You may obtain a copy of the License at                                    #
 #                                                                            #
 #     http://www.apache.org/licenses/LICENSE-2.0                             #
 #                                                                            #
 # Unless required by applicable law or agreed to in writing, software        #
 # distributed under the License is distributed on an "AS IS" BASIS,          #
 # WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.   #
 # See the License for the specific language governing permissions and        #
 # limitations under the License.                                             #
 #                                                                            #
 ##############################################################################

macro(core_test test)
    add_executable(${test} ${test}.cpp)
    target_link_libraries(${test} testing)
    target_include_directories(${test} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    set_target_properties(${test} PROPERTIES CXX_CLANG_TIDY "${VCML_LINTER}")
    add_test(NAME core/${test} COMMAND ${test} ${VCML_TEST_RESOURCES})
    set_tests_properties(core/${test} PROPERTIES ENVIRONMENT "${VCML_ENVVAR}")
    set_tests_properties(core/${test} PROPERTIES TIMEOUT 4)
endmacro()

core_test("hello")
core_test("sysc")
core_test("logging")
core_test("version")
core_test("dmi")
core_test("range")
core_test("exmon")
core_test("property")
core_test("module")
core_test("component")
core_test("peripheral")
core_test("register")
core_test("processor")
core_test("gpio")
core_test("clk")
core_test("spi")
core_test("sd")
core_test("i2c")
core_test("pci")
core_test("eth")
core_test("can")
core_test("serial")
core_test("adapter")
core_test("virtio")
core_test("display")
core_test("symtab")
core_test("thctl")
core_test("suspender")
core_test("async")
core_test("stubs")
core_test("tracing")
core_test("timer")
core_test("memory")
