#ifndef METHODDISPATCH_HH
#define METHODDISPATCH_HH

namespace AODBC {

template <
    typename OwnerT,
    typename ResultT,
    typename MethodT,
    typename ArgsTuple
>
ResultT call_method(MethodT tag, OwnerT* owner, ArgsTuple args);


}  // namespace AODBC

#endif  /* METHODDISPATCH_HH */
