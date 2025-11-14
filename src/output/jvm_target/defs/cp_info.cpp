#include "cp_info.h"

codesh::output::jvm_target::defs::cp_info::cp_info(const unsigned char tag) : tag(tag)
{}

codesh::output::jvm_target::defs::cp_info::~cp_info() = default;


codesh::output::jvm_target::defs::CONSTANT_Utf8_info::CONSTANT_Utf8_info() :
    cp_info(1)
{}
codesh::output::jvm_target::defs::CONSTANT_Integer_info::CONSTANT_Integer_info() :
    cp_info(3)
{}
codesh::output::jvm_target::defs::CONSTANT_Float_info::CONSTANT_Float_info() :
    cp_info(4)
{}
codesh::output::jvm_target::defs::CONSTANT_Long_info::CONSTANT_Long_info() :
    cp_info(5)
{}
codesh::output::jvm_target::defs::CONSTANT_Double_info::CONSTANT_Double_info() :
    cp_info(6)
{}
codesh::output::jvm_target::defs::CONSTANT_Class_info::CONSTANT_Class_info() :
    cp_info(7)
{}
codesh::output::jvm_target::defs::CONSTANT_String_info::CONSTANT_String_info() :
    cp_info(8)
{}
codesh::output::jvm_target::defs::CONSTANT_Fieldref_info::CONSTANT_Fieldref_info() :
    cp_info(9)
{}
codesh::output::jvm_target::defs::CONSTANT_Methodref_info::CONSTANT_Methodref_info() :
    cp_info(10)
{}
codesh::output::jvm_target::defs::CONSTANT_InterfaceMethodref_info::CONSTANT_InterfaceMethodref_info() :
    cp_info(11)
{}
codesh::output::jvm_target::defs::CONSTANT_NameAndType_info::CONSTANT_NameAndType_info() :
    cp_info(12)
{}