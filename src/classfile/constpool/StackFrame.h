//
// Created by xman on 6/29/2022.
//

#ifndef TARRASH_STACKFRAME_H
#define TARRASH_STACKFRAME_H

#include <vector>
#include "classfile_constants.h"
#include "StructsCommon.h"

namespace org::kapa::tarracsh::stack {

using common::u1;
using common::u2;


struct SameFrame {
    // u1 frame_type = SAME;
    u1 frameType;
};

struct TopVariableInfo {
    u1 tag = JVM_ITEM_Top;
};

struct IntegerVariableInfo {
    u1 tag = JVM_ITEM_Integer;
};

struct FloatVariableInfo {
    u1 tag = JVM_ITEM_Float;
};

struct LongVariableInfo {
    u1 tag = JVM_ITEM_Long;
};

struct DoubleVariableInfo {
    u1 tag = JVM_ITEM_Double;
};

struct NullVariableInfo {
    u1 tag = JVM_ITEM_Null;
};

struct UninitializedThisVariableInfo {
    u1 tag = JVM_ITEM_UninitializedThis;
};

struct ObjectVariableInfo {
    u1 tag = JVM_ITEM_Object;
    u2 cpool_index;
};

struct UninitializedVariableInfo {
    u1 tag = JVM_ITEM_Uninitialized;
    u2 offset;
};

struct VerificationTypeInfo {
    union {
        TopVariableInfo topVariableInfo;
        IntegerVariableInfo variableInfo;
        FloatVariableInfo floatVariableInfo;
        LongVariableInfo longVariableInfo;
        DoubleVariableInfo doubleVariableInfo;
        NullVariableInfo nullVariableInfo;
        UninitializedThisVariableInfo uninitializedThisVariableInfo;
        ObjectVariableInfo objectVariableInfo;
        UninitializedVariableInfo uninitializedVariableInfo;
    };
};

struct SameLocals1StackItemFrame {
    //    u1 frame_type = SAME_LOCALS_1_STACK_ITEM; /* 64-127 */
    u1 frame_type;
    VerificationTypeInfo stack[1];
};

struct SameLocals1StackItemFrameExtended {
    // u1 frame_type = SAME_LOCALS_1_STACK_ITEM_EXTENDED; /* 247 */
    u1 frame_type;
    u2 offset_delta;
    VerificationTypeInfo stack[1];
};

struct ChopFrame {
    // u1 frame_type = CHOP; /* 248-250 */
    u1 frame_type;
    u2 offset_delta;
};

struct SameFrameExtended {
    // u1 frame_type = SAME_FRAME_EXTENDED; /* 251 */
    u1 frame_type;
    u2 offset_delta;
};

struct AppendFrame {
    //    u1 frame_type = APPEND; /* 252-254 */
    u1 frame_type;
    u2 offset_delta;
    std::vector<VerificationTypeInfo> verificationTypeInfos;
    //verification_type_info locals[frame_type - 251];
    /**

    The 0th entry in locals represents the type of the first additional local variable. If locals[M] represents local
    variable N, then locals[M+1] represents local variable N+1 if locals[M] is one of:

    Top_variable_info
    Integer_variable_info
    Float_variable_info
    Null_variable_info
    UninitializedThis_variable_info
    Object_variable_info
    Uninitialized_variable_info
    Otherwise locals[M+1] represents local variable N+2.
    */
};

struct FullFrame {
    // u1 frame_type = FULL_FRAME; /* 255 */
    u1 frame_type;
    u2 offset_delta;
    u2 number_of_locals;
    // verification_type_info locals[number_of_locals];
    std::vector<VerificationTypeInfo> verificationTypeInfoLocals;
    u2 number_of_stack_items;
    // verification_type_info stack[number_of_stack_items];
    std::vector<VerificationTypeInfo> verificationTypeInfos;
    /**
    The 0th entry in locals represents the type of local variable 0. If locals[M] represents local variable N, then
    locals[M+1] represents local variable N+1 if locals[M] is one of:

    Top_variable_info
    Integer_variable_info
    Float_variable_info
    Null_variable_info
    UninitializedThis_variable_info
    Object_variable_info
    Uninitialized_variable_info
    Otherwise locals[M+1] represents local variable N+2.

    */
};

struct StackMapFrame {
    union {
        SameFrame sameFrame{0};
        SameLocals1StackItemFrame sameLocals1StackItemFrame;
        SameLocals1StackItemFrameExtended sameLocals1StackItemFrameExtended;
        ChopFrame chopFrame;
        SameFrameExtended sameFrameExtended;
        AppendFrame appendFrame;
        FullFrame fullFrame;
    };

    StackMapFrame() = delete;
    ~StackMapFrame() = delete;
};

}

#endif // TARRASH_STACKFRAME_H
