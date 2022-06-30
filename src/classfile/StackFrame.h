//
// Created by xman on 6/29/2022.
//

#ifndef TARRASH_STACKFRAME_H
#define TARRASH_STACKFRAME_H

#include "StructsCommon.h"

using namespace org::kapa::tarrash::common;

namespace org {
namespace kapa {
namespace tarrash {
namespace stack {
//
//struct same_frame {
//    // u1 frame_type = SAME; /* 0-63 */
//    u1 frameType;
//};
//
//struct Top_variable_info {
//    u1 tag = ITEM_Top; /* 0 */
//};
//
//struct Integer_variable_info {
//    u1 tag = ITEM_Integer; /* 1 */
//};
//
//struct Float_variable_info {
//    u1 tag = ITEM_Float; /* 2 */
//};
//
//struct Long_variable_info {
//    u1 tag = ITEM_Long; /* 4 */
//};
//
//struct Double_variable_info {
//    u1 tag = ITEM_Double; /* 3 */
//};
//
//struct Null_variable_info {
//    u1 tag = ITEM_Null; /* 5 */
//};
//
//struct UninitializedThis_variable_info {
//    u1 tag = ITEM_UninitializedThis; /* 6 */
//};
//
//struct Object_variable_info {
//    u1 tag = ITEM_Object; /* 7 */
//    u2 cpool_index;
//};
//
//struct Uninitialized_variable_info {
//    u1 tag = ITEM_Uninitialized /* 8 */
//        u2 offset;
//}
//
//struct verification_type_info {
//    union {
//        Top_variable_info;
//        Integer_variable_info;
//        Float_variable_info;
//        Long_variable_info;
//        Double_variable_info;
//        Null_variable_info;
//        UninitializedThis_variable_info;
//        Object_variable_info;
//        Uninitialized_variable_info;
//    }
//};
//
//struct same_locals_1_stack_item_frame {
//    //    u1 frame_type = SAME_LOCALS_1_STACK_ITEM; /* 64-127 */
//    u1 frame_type;
//    verification_type_info stack[1];
//};
//
//struct same_locals_1_stack_item_frame_extended {
//    // u1 frame_type = SAME_LOCALS_1_STACK_ITEM_EXTENDED; /* 247 */
//    u1 frame_type;
//    u2 offset_delta;
//    verification_type_info stack[1];
//};
//
//struct chop_frame {
//    // u1 frame_type = CHOP; /* 248-250 */
//    u1 frame_type;
//    u2 offset_delta;
//};
//
//struct same_frame_extended {
//    // u1 frame_type = SAME_FRAME_EXTENDED; /* 251 */
//    u1 frame_type;
//    u2 offset_delta;
//};
//
//struct append_frame {
//    //    u1 frame_type = APPEND; /* 252-254 */
//    u1 frame_type;
//    u2 offset_delta;
//    verification_type_info locals[frame_type - 251];
//    /**
//
//    The 0th entry in locals represents the type of the first additional local variable. If locals[M] represents local
//    variable N, then locals[M+1] represents local variable N+1 if locals[M] is one of:
//
//    Top_variable_info
//    Integer_variable_info
//    Float_variable_info
//    Null_variable_info
//    UninitializedThis_variable_info
//    Object_variable_info
//    Uninitialized_variable_info
//    Otherwise locals[M+1] represents local variable N+2.
//    */
//}
//
//struct full_frame {
//    // u1 frame_type = FULL_FRAME; /* 255 */
//    u1 frame_type;
//    u2 offset_delta;
//    u2 number_of_locals;
//    verification_type_info locals[number_of_locals];
//    u2 number_of_stack_items;
//    verification_type_info stack[number_of_stack_items];
//    /**
//    The 0th entry in locals represents the type of local variable 0. If locals[M] represents local variable N, then
//    locals[M+1] represents local variable N+1 if locals[M] is one of:
//
//    Top_variable_info
//    Integer_variable_info
//    Float_variable_info
//    Null_variable_info
//    UninitializedThis_variable_info
//    Object_variable_info
//    Uninitialized_variable_info
//    Otherwise locals[M+1] represents local variable N+2.
//
//    */
//}
//
//struct StackMapFrame {
//    union {
//        same_frame;
//        same_locals_1_stack_item_frame;
//        same_locals_1_stack_item_frame_extended;
//        chop_frame;
//        same_frame_extended;
//        append_frame;
//        full_frame;
//    }
//};

} // namespace stack
} // namespace tarrash
} // namespace kapa
} // namespace org
#endif // TARRASH_STACKFRAME_H
