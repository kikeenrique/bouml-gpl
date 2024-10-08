
#include "UmlCom.h"
#include "UmlBaseActivityActionClasses.h"
#include "UmlActivityActionClasses.h"
#include "UmlItem.h"
#include "UmlOperation.h"

UmlSendObjectAction * UmlBaseSendObjectAction::create (UmlItem * parent, const char * s)
{
    return (UmlSendObjectAction *) parent->create_ (aSendObjectAction, s);
}

anItemKind UmlBaseSendObjectAction::kind()
{
    return aSendObjectAction;
}

UmlUnmarshallAction * UmlBaseUnmarshallAction::create (UmlItem * parent, const char * s)
{
    return (UmlUnmarshallAction *) parent->create_ (anUnmarshallAction, s);
}

anItemKind UmlBaseUnmarshallAction::kind()
{
    return anUnmarshallAction;
}

UmlSendSignalAction * UmlBaseSendSignalAction::create (UmlItem * parent, const char * s)
{
    return (UmlSendSignalAction *) parent->create_ (aSendSignalAction, s);
}

anItemKind UmlBaseSendSignalAction::kind()
{
    return aSendSignalAction;
}

UmlBroadcastSignalAction * UmlBaseBroadcastSignalAction::create (UmlItem * parent, const char * s)
{
    return (UmlBroadcastSignalAction *) parent->create_ (aBroadcastSignalAction, s);
}

anItemKind UmlBaseBroadcastSignalAction::kind()
{
    return aBroadcastSignalAction;
}

UmlValueSpecificationAction * UmlBaseValueSpecificationAction::create (UmlItem * parent, const char * s)
{
    return (UmlValueSpecificationAction *) parent->create_ (aValueSpecificationAction, s);
}

anItemKind UmlBaseValueSpecificationAction::kind()
{
    return aValueSpecificationAction;
}

const QCString & UmlBaseValueSpecificationAction::value()
{
    read_if_needed_();
    return _value;
}

bool UmlBaseValueSpecificationAction::set_Value (const char * v)
{
    return set_it_ (_value, v, setUmlActivityCmd);
}

#ifdef WITHCPP
const QCString & UmlBaseValueSpecificationAction::cppValue()
{
    read_if_needed_();
    return _cpp_value;
}

bool UmlBaseValueSpecificationAction::set_CppValue (const char * v)
{
    return set_it_ (_cpp_value, v, setCppActivityCmd);
}
#endif

#ifdef WITHJAVA
const QCString & UmlBaseValueSpecificationAction::javaValue()
{
    read_if_needed_();
    return _java_value;
}

bool UmlBaseValueSpecificationAction::set_JavaValue (const char * v)
{
    return set_it_ (_java_value, v, setJavaActivityCmd);
}
#endif

void UmlBaseValueSpecificationAction::unload (bool rec, bool del)
{
    _value = 0;
#ifdef WITHCPP
    _cpp_value = 0;
#endif
#ifdef WITHJAVA
    _java_value = 0;
#endif
    UmlBaseActivityAction::unload (rec, del);
}

void UmlBaseValueSpecificationAction::read_uml_()
{
    UmlBaseActivityAction::read_uml_();
    _value = UmlCom::read_string();
}

#ifdef WITHCPP
void UmlBaseValueSpecificationAction::read_cpp_()
{
    UmlBaseActivityAction::read_cpp_();
    _cpp_value = UmlCom::read_string();
}
#endif

#ifdef WITHJAVA
void UmlBaseValueSpecificationAction::read_java_()
{
    UmlBaseActivityAction::read_java_();
    _java_value = UmlCom::read_string();
}
#endif

UmlOpaqueAction * UmlBaseOpaqueAction::create (UmlItem * parent, const char * s)
{
    return (UmlOpaqueAction *) parent->create_ (anOpaqueAction, s);
}

anItemKind UmlBaseOpaqueAction::kind()
{
    return anOpaqueAction;
}

const QCString & UmlBaseOpaqueAction::behavior()
{
    read_if_needed_();
    return _behavior;
}

bool UmlBaseOpaqueAction::set_Behavior (const char * v)
{
    return set_it_ (_behavior, v, setUmlActivityCmd);
}

#ifdef WITHCPP
const QCString & UmlBaseOpaqueAction::cppBehavior()
{
    read_if_needed_();
    return _cpp_behavior;
}

bool UmlBaseOpaqueAction::set_CppBehavior (const char * v)
{
    return set_it_ (_cpp_behavior, v, setCppActivityCmd);
}
#endif

#ifdef WITHJAVA
const QCString & UmlBaseOpaqueAction::javaBehavior()
{
    read_if_needed_();
    return _java_behavior;
}

bool UmlBaseOpaqueAction::set_JavaBehavior (const char * v)
{
    return set_it_ (_java_behavior, v, setJavaActivityCmd);
}
#endif

void UmlBaseOpaqueAction::unload (bool rec, bool del)
{
    _behavior = 0;
#ifdef WITHCPP
    _cpp_behavior = 0;
#endif
#ifdef WITHJAVA
    _java_behavior = 0;
#endif
    UmlBaseActivityAction::unload (rec, del);
}

void UmlBaseOpaqueAction::read_uml_()
{
    UmlBaseActivityAction::read_uml_();
    _behavior = UmlCom::read_string();
}

#ifdef WITHCPP
void UmlBaseOpaqueAction::read_cpp_()
{
    UmlBaseActivityAction::read_cpp_();
    _cpp_behavior = UmlCom::read_string();
}
#endif

#ifdef WITHJAVA
void UmlBaseOpaqueAction::read_java_()
{
    UmlBaseActivityAction::read_java_();
    _java_behavior = UmlCom::read_string();
}
#endif

UmlAcceptEventAction * UmlBaseAcceptEventAction::create (UmlItem * parent, const char * s)
{
    return (UmlAcceptEventAction *) parent->create_ (anAcceptEventAction, s);
}

anItemKind UmlBaseAcceptEventAction::kind()
{
    return anAcceptEventAction;
}

bool UmlBaseAcceptEventAction::isUnmarshall()
{
    read_if_needed_();
    return _unmarshall;
}

bool UmlBaseAcceptEventAction::set_isUnmarshall (bool v)
{
    return set_it_ (_unmarshall, v, setUnmarshallCmd);
}

bool UmlBaseAcceptEventAction::isTimeEvent()
{
    read_if_needed_();
    return _timeevent;
}

bool UmlBaseAcceptEventAction::set_isTimeEvent (bool v)
{
    return set_it_ (_timeevent, v, setTimeEventCmd);
}

const QCString & UmlBaseAcceptEventAction::trigger()
{
    read_if_needed_();
    return _trigger;
}

bool UmlBaseAcceptEventAction::set_Trigger (const char * v)
{
    return set_it_ (_trigger, v, setUmlTriggerCmd);
}

#ifdef WITHCPP
const QCString & UmlBaseAcceptEventAction::cppTrigger()
{
    read_if_needed_();
    return _cpp_trigger;
}

bool UmlBaseAcceptEventAction::set_CppTrigger (const char * v)
{
    return set_it_ (_cpp_trigger, v, setCppTriggerCmd);
}
#endif

#ifdef WITHJAVA
const QCString & UmlBaseAcceptEventAction::javaTrigger()
{
    read_if_needed_();
    return _java_trigger;
}

bool UmlBaseAcceptEventAction::set_JavaTrigger (const char * v)
{
    return set_it_ (_java_trigger, v, setJavaTriggerCmd);
}
#endif

void UmlBaseAcceptEventAction::unload (bool rec, bool del)
{
    _trigger = 0;
#ifdef WITHCPP
    _cpp_trigger = 0;
#endif
#ifdef WITHJAVA
    _java_trigger = 0;
#endif
    UmlBaseActivityAction::unload (rec, del);
}

void UmlBaseAcceptEventAction::read_uml_()
{
    UmlBaseActivityAction::read_uml_();
    _unmarshall = UmlCom::read_bool();
    _timeevent = UmlCom::read_bool();
    _trigger = UmlCom::read_string();
}

#ifdef WITHCPP
void UmlBaseAcceptEventAction::read_cpp_()
{
    UmlBaseActivityAction::read_cpp_();
    _cpp_trigger = UmlCom::read_string();
}
#endif

#ifdef WITHJAVA
void UmlBaseAcceptEventAction::read_java_()
{
    UmlBaseActivityAction::read_java_();
    _java_trigger = UmlCom::read_string();
}
#endif

UmlCallOperationAction * UmlBaseCallOperationAction::create (UmlItem * parent, const char * s)
{
    return (UmlCallOperationAction *) parent->create_ (aCallOperationAction, s);
}

anItemKind UmlBaseCallOperationAction::kind()
{
    return aCallOperationAction;
}

bool UmlBaseCallOperationAction::isSynchronous()
{
    read_if_needed_();
    return _synchronous;
}

bool UmlBaseCallOperationAction::set_isSynchronous (bool v)
{
    return set_it_ (_synchronous, v, setFlagCmd);
}

UmlOperation * UmlBaseCallOperationAction::operation()
{
    read_if_needed_();
    return _operation;
}

bool UmlBaseCallOperationAction::set_Operation (UmlOperation * v)
{
    UmlCom::send_cmd (_identifier, setDefCmd, ( (UmlBaseItem *) v)->_identifier);
    if (UmlCom::read_bool()) {
        _operation = v;
        return TRUE;
    }   else {
        return FALSE;
    }
}

void UmlBaseCallOperationAction::read_uml_()
{
    UmlBaseActivityAction::read_uml_();
    _synchronous = UmlCom::read_bool();
    _operation = (UmlOperation *) UmlBaseItem::read_();
}

UmlCallBehaviorAction * UmlBaseCallBehaviorAction::create (UmlItem * parent, const char * s)
{
    return (UmlCallBehaviorAction *) parent->create_ (aCallBehaviorAction, s);
}

anItemKind UmlBaseCallBehaviorAction::kind()
{
    return aCallBehaviorAction;
}

bool UmlBaseCallBehaviorAction::isSynchronous()
{
    read_if_needed_();
    return _synchronous;
}

bool UmlBaseCallBehaviorAction::set_isSynchronous (bool v)
{
    return set_it_ (_synchronous, v, setFlagCmd);
}

UmlItem * UmlBaseCallBehaviorAction::behavior()
{
    read_if_needed_();
    return _behavior;
}

bool UmlBaseCallBehaviorAction::set_Behavior (UmlItem * v)
{
    UmlCom::send_cmd (_identifier, setDefCmd, ( (UmlBaseItem *) v)->_identifier);
    if (UmlCom::read_bool()) {
        _behavior = v;
        return TRUE;
    }   else {
        return FALSE;
    }
}

void UmlBaseCallBehaviorAction::read_uml_()
{
    UmlBaseActivityAction::read_uml_();
    _synchronous = UmlCom::read_bool();
    _behavior = UmlBaseItem::read_();
}

UmlClearVariableValueAction * UmlBaseClearVariableValueAction::create (UmlItem * parent, const char * s)
{
    return (UmlClearVariableValueAction *) parent->create_ (aClearVariableValueAction, s);
}

anItemKind UmlBaseClearVariableValueAction::kind()
{
    return aClearVariableValueAction;
}

UmlReadVariableValueAction * UmlBaseReadVariableValueAction::create (UmlItem * parent, const char * s)
{
    return (UmlReadVariableValueAction *) parent->create_ (aReadVariableValueAction, s);
}

anItemKind UmlBaseReadVariableValueAction::kind()
{
    return aReadVariableValueAction;
}

UmlWriteVariableValueAction * UmlBaseWriteVariableValueAction::create (UmlItem * parent, const char * s)
{
    return (UmlWriteVariableValueAction *) parent->create_ (aWriteVariableValueAction, s);
}

anItemKind UmlBaseWriteVariableValueAction::kind()
{
    return aWriteVariableValueAction;
}

UmlAddVariableValueAction * UmlBaseAddVariableValueAction::create (UmlItem * parent, const char * s)
{
    return (UmlAddVariableValueAction *) parent->create_ (anAddVariableValueAction, s);
}

anItemKind UmlBaseAddVariableValueAction::kind()
{
    return anAddVariableValueAction;
}

bool UmlBaseAddVariableValueAction::isReplaceAll()
{
    read_if_needed_();
    return _replace_all;
}

bool UmlBaseAddVariableValueAction::set_isReplaceAll (bool v)
{
    return set_it_ (_replace_all, v, setFlagCmd);
}

void UmlBaseAddVariableValueAction::read_uml_()
{
    UmlBaseAccessVariableValueAction::read_uml_();
    _replace_all = UmlCom::read_bool();
}

UmlRemoveVariableValueAction * UmlBaseRemoveVariableValueAction::create (UmlItem * parent, const char * s)
{
    return (UmlRemoveVariableValueAction *) parent->create_ (aRemoveVariableValueAction, s);
}

anItemKind UmlBaseRemoveVariableValueAction::kind()
{
    return aRemoveVariableValueAction;
}

bool UmlBaseRemoveVariableValueAction::isRemoveDuplicates()
{
    read_if_needed_();
    return _remove_duplicates;
}

bool UmlBaseRemoveVariableValueAction::set_isRemoveDuplicates (bool v)
{
    return set_it_ (_remove_duplicates, v, setFlagCmd);
}

void UmlBaseRemoveVariableValueAction::read_uml_()
{
    UmlBaseAccessVariableValueAction::read_uml_();
    _remove_duplicates = UmlCom::read_bool();
}

UmlAcceptCallAction * UmlBaseAcceptCallAction::create (UmlItem * parent, const char * s)
{
    return (UmlAcceptCallAction *) parent->create_ (anAcceptCallAction, s);
}

anItemKind UmlBaseAcceptCallAction::kind()
{
    return anAcceptCallAction;
}

const QCString & UmlBaseAcceptCallAction::trigger()
{
    read_if_needed_();
    return _trigger;
}

bool UmlBaseAcceptCallAction::set_Trigger (const char * v)
{
    return set_it_ (_trigger, v, setUmlTriggerCmd);
}

#ifdef WITHCPP
const QCString & UmlBaseAcceptCallAction::cppTrigger()
{
    read_if_needed_();
    return _cpp_trigger;
}

bool UmlBaseAcceptCallAction::set_CppTrigger (const char * v)
{
    return set_it_ (_cpp_trigger, v, setCppTriggerCmd);
}
#endif

#ifdef WITHJAVA
const QCString & UmlBaseAcceptCallAction::javaTrigger()
{
    read_if_needed_();
    return _java_trigger;
}

bool UmlBaseAcceptCallAction::set_JavaTrigger (const char * v)
{
    return set_it_ (_java_trigger, v, setJavaTriggerCmd);
}
#endif

void UmlBaseAcceptCallAction::unload (bool rec, bool del)
{
    _trigger = 0;
#ifdef WITHCPP
    _cpp_trigger = 0;
#endif
#ifdef WITHJAVA
    _java_trigger = 0;
#endif
    UmlBaseActivityAction::unload (rec, del);
}

void UmlBaseAcceptCallAction::read_uml_()
{
    UmlBaseActivityAction::read_uml_();
    _trigger = UmlCom::read_string();
}

#ifdef WITHCPP
void UmlBaseAcceptCallAction::read_cpp_()
{
    UmlBaseActivityAction::read_cpp_();
    _cpp_trigger = UmlCom::read_string();
}
#endif

#ifdef WITHJAVA
void UmlBaseAcceptCallAction::read_java_()
{
    UmlBaseActivityAction::read_java_();
    _java_trigger = UmlCom::read_string();
}
#endif

UmlReplyAction * UmlBaseReplyAction::create (UmlItem * parent, const char * s)
{
    return (UmlReplyAction *) parent->create_ (aReplyAction, s);
}

anItemKind UmlBaseReplyAction::kind()
{
    return aReplyAction;
}

const QCString & UmlBaseReplyAction::replyToCall()
{
    read_if_needed_();
    return _trigger;
}

bool UmlBaseReplyAction::set_ReplyToCall (const char * v)
{
    return set_it_ (_trigger, v, setUmlTriggerCmd);
}

#ifdef WITHCPP
const QCString & UmlBaseReplyAction::cppReplyToCall()
{
    read_if_needed_();
    return _cpp_trigger;
}

bool UmlBaseReplyAction::set_CppReplyToCall (const char * v)
{
    return set_it_ (_cpp_trigger, v, setCppTriggerCmd);
}
#endif

#ifdef WITHJAVA
const QCString & UmlBaseReplyAction::javaReplyToCall()
{
    read_if_needed_();
    return _java_trigger;
}

bool UmlBaseReplyAction::set_JavaReplyToCall (const char * v)
{
    return set_it_ (_java_trigger, v, setJavaTriggerCmd);
}
#endif

void UmlBaseReplyAction::unload (bool rec, bool del)
{
    _trigger = 0;
#ifdef WITHCPP
    _cpp_trigger = 0;
#endif
#ifdef WITHJAVA
    _java_trigger = 0;
#endif
    UmlBaseActivityAction::unload (rec, del);
}

void UmlBaseReplyAction::read_uml_()
{
    UmlBaseActivityAction::read_uml_();
    _trigger = UmlCom::read_string();
}

#ifdef WITHCPP
void UmlBaseReplyAction::read_cpp_()
{
    UmlBaseActivityAction::read_cpp_();
    _cpp_trigger = UmlCom::read_string();
}
#endif

#ifdef WITHJAVA
void UmlBaseReplyAction::read_java_()
{
    UmlBaseActivityAction::read_java_();
    _java_trigger = UmlCom::read_string();
}
#endif

UmlCreateObjectAction * UmlBaseCreateObjectAction::create (UmlItem * parent, const char * s)
{
    return (UmlCreateObjectAction *) parent->create_ (aCreateObjectAction, s);
}

anItemKind UmlBaseCreateObjectAction::kind()
{
    return aCreateObjectAction;
}

const QCString & UmlBaseCreateObjectAction::classifier()
{
    read_if_needed_();
    return _classifier;
}

bool UmlBaseCreateObjectAction::set_Classifier (const char * v)
{
    return set_it_ (_classifier, v, setDefCmd);
}

void UmlBaseCreateObjectAction::unload (bool rec, bool del)
{
    _classifier = 0;
    UmlBaseActivityAction::unload (rec, del);
}

void UmlBaseCreateObjectAction::read_uml_()
{
    UmlBaseActivityAction::read_uml_();
    _classifier = UmlCom::read_string();
}

UmlDestroyObjectAction * UmlBaseDestroyObjectAction::create (UmlItem * parent, const char * s)
{
    return (UmlDestroyObjectAction *) parent->create_ (aDestroyObjectAction, s);
}

anItemKind UmlBaseDestroyObjectAction::kind()
{
    return aDestroyObjectAction;
}

bool UmlBaseDestroyObjectAction::isDestroyLinks()
{
    read_if_needed_();
    return _links;
}

bool UmlBaseDestroyObjectAction::set_isDestroyLinks (bool v)
{
    return set_it_ (_links, v, setTypeCmd);
}

bool UmlBaseDestroyObjectAction::isDestroyOwnedObjects()
{
    read_if_needed_();
    return _owned_objects;
}

bool UmlBaseDestroyObjectAction::set_isDestroyOwnedObjects (bool v)
{
    return set_it_ (_owned_objects, v, setFlagCmd);
}

void UmlBaseDestroyObjectAction::read_uml_()
{
    UmlBaseActivityAction::read_uml_();
    _links = UmlCom::read_bool();
    _owned_objects = UmlCom::read_bool();
}

UmlTestIdentityAction * UmlBaseTestIdentityAction::create (UmlItem * parent, const char * s)
{
    return (UmlTestIdentityAction *) parent->create_ (aTestIdentityAction, s);
}

anItemKind UmlBaseTestIdentityAction::kind()
{
    return aTestIdentityAction;
}

UmlRaiseExceptionAction * UmlBaseRaiseExceptionAction::create (UmlItem * parent, const char * s)
{
    return (UmlRaiseExceptionAction *) parent->create_ (aRaiseExceptionAction, s);
}

anItemKind UmlBaseRaiseExceptionAction::kind()
{
    return aRaiseExceptionAction;
}

UmlReduceAction * UmlBaseReduceAction::create (UmlItem * parent, const char * s)
{
    return (UmlReduceAction *) parent->create_ (aReduceAction, s);
}

anItemKind UmlBaseReduceAction::kind()
{
    return aReduceAction;
}

bool UmlBaseReduceAction::isOrdered()
{
    read_if_needed_();
    return _ordered;
}

bool UmlBaseReduceAction::set_isOrdered (bool v)
{
    return set_it_ (_ordered, v, setFlagCmd);
}

UmlItem * UmlBaseReduceAction::reducer()
{
    read_if_needed_();
    return _reducer;
}

bool UmlBaseReduceAction::set_Reducer (UmlItem * v)
{
    UmlCom::send_cmd (_identifier, setDefCmd, (v == 0) ? (void *) v : ( (UmlBaseItem *) v)->_identifier);
    if (UmlCom::read_bool()) {
        _reducer = v;
        return TRUE;
    } else {
        return FALSE;
    }
}

void UmlBaseReduceAction::read_uml_()
{
    UmlBaseActivityAction::read_uml_();
    _ordered = UmlCom::read_bool();
    _reducer = UmlBaseItem::read_();
}

