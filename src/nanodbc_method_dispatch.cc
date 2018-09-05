#include "nanobdc_method_dispatch.hh"

namespace AODBC {

template<>
nanodbc::string call_method(
        MethodTag<CommandNames::dbms_name>,
        UVMonitor<nanodbc::connection>* owner,
        std::tuple<> tuple) {
    Synchronized lock {owner};
    return owner->get()->dbms_name();
}

}
