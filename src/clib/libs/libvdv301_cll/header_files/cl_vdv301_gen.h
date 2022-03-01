
#ifndef CL_LIB_VDV301_GEN_H
#define CL_LIB_VDV301_GEN_H

@begin
include "cl_vdv301_basic.h"
@end

// === definition of generated structures ======================================

// VDV301 GENERATED DECLARATIONS

// -- ConnectionStateEnumeration_s --
@begin
struct
<
ui:value
>
ConnectionStateEnumeration_s;
@end

enum {
  ConnectionStateEnumeration_ConnectionBroken = 1,
  ConnectionStateEnumeration_ConnectionOK = 2,
  ConnectionStateEnumeration_NoInformationAvailable = 3,
};

extern string_tree_s g_ConnectionStateEnumeration_tree;

WUR int ConnectionStateEnumeration_s_write(ConnectionStateEnumeration_s *this,bc_array_s *a_trg);
WUR int ConnectionStateEnumeration_s_read(ConnectionStateEnumeration_s *this,var_s a_var);

// -- ConnectionTypeEnumeration_s --
@begin
struct
<
ui:value
>
ConnectionTypeEnumeration_s;
@end

enum {
  ConnectionTypeEnumeration_Interchange = 1,
  ConnectionTypeEnumeration_ProtectedConnection = 2,
};

extern string_tree_s g_ConnectionTypeEnumeration_tree;

WUR int ConnectionTypeEnumeration_s_write(ConnectionTypeEnumeration_s *this,bc_array_s *a_trg);
WUR int ConnectionTypeEnumeration_s_read(ConnectionTypeEnumeration_s *this,var_s a_var);

// -- DoorCountingObjectClassEnumeration_s --
@begin
struct
<
ui:value
>
DoorCountingObjectClassEnumeration_s;
@end

enum {
  DoorCountingObjectClassEnumeration_Adult = 1,
  DoorCountingObjectClassEnumeration_Child = 2,
  DoorCountingObjectClassEnumeration_Bike = 3,
  DoorCountingObjectClassEnumeration_WheelChair = 4,
  DoorCountingObjectClassEnumeration_Pram = 5,
  DoorCountingObjectClassEnumeration_Unidentified = 6,
  DoorCountingObjectClassEnumeration_Other = 7,
};

extern string_tree_s g_DoorCountingObjectClassEnumeration_tree;

WUR int DoorCountingObjectClassEnumeration_s_write(DoorCountingObjectClassEnumeration_s *this,bc_array_s *a_trg);
WUR int DoorCountingObjectClassEnumeration_s_read(DoorCountingObjectClassEnumeration_s *this,var_s a_var);

// -- DoorCountingQualityEnumeration_s --
@begin
struct
<
ui:value
>
DoorCountingQualityEnumeration_s;
@end

enum {
  DoorCountingQualityEnumeration_Defect = 1,
  DoorCountingQualityEnumeration_Regular = 2,
  DoorCountingQualityEnumeration_Sabotage = 3,
  DoorCountingQualityEnumeration_Other = 4,
};

extern string_tree_s g_DoorCountingQualityEnumeration_tree;

WUR int DoorCountingQualityEnumeration_s_write(DoorCountingQualityEnumeration_s *this,bc_array_s *a_trg);
WUR int DoorCountingQualityEnumeration_s_read(DoorCountingQualityEnumeration_s *this,var_s a_var);

// -- DoorOpenStateEnumeration_s --
@begin
struct
<
ui:value
>
DoorOpenStateEnumeration_s;
@end

enum {
  DoorOpenStateEnumeration_DoorsOpen = 1,
  DoorOpenStateEnumeration_AllDoorsClosed = 2,
  DoorOpenStateEnumeration_SingleDoorOpen = 3,
  DoorOpenStateEnumeration_SingleDoorClosed = 4,
};

extern string_tree_s g_DoorOpenStateEnumeration_tree;

WUR int DoorOpenStateEnumeration_s_write(DoorOpenStateEnumeration_s *this,bc_array_s *a_trg);
WUR int DoorOpenStateEnumeration_s_read(DoorOpenStateEnumeration_s *this,var_s a_var);

// -- DoorOperationStateEnumeration_s --
@begin
struct
<
ui:value
>
DoorOperationStateEnumeration_s;
@end

enum {
  DoorOperationStateEnumeration_Locked = 1,
  DoorOperationStateEnumeration_Normal = 2,
  DoorOperationStateEnumeration_EmergencyRelease = 3,
};

extern string_tree_s g_DoorOperationStateEnumeration_tree;

WUR int DoorOperationStateEnumeration_s_write(DoorOperationStateEnumeration_s *this,bc_array_s *a_trg);
WUR int DoorOperationStateEnumeration_s_read(DoorOperationStateEnumeration_s *this,var_s a_var);

// -- ErrorCodeEnumeration_s --
@begin
struct
<
ui:value
>
ErrorCodeEnumeration_s;
@end

enum {
  ErrorCodeEnumeration_DataEstimated = 1,
  ErrorCodeEnumeration_FaultData = 2,
  ErrorCodeEnumeration_NoScheduleDataAvailable = 3,
  ErrorCodeEnumeration_DeviceMissing = 4,
  ErrorCodeEnumeration_NoServiceResponse = 5,
  ErrorCodeEnumeration_ImportantDataNotAvailable = 6,
  ErrorCodeEnumeration_DataNotValid = 7,
};

extern string_tree_s g_ErrorCodeEnumeration_tree;

WUR int ErrorCodeEnumeration_s_write(ErrorCodeEnumeration_s *this,bc_array_s *a_trg);
WUR int ErrorCodeEnumeration_s_read(ErrorCodeEnumeration_s *this,var_s a_var);

// -- TicketRazziaInformationEnumeration_s --
@begin
struct
<
ui:value
>
TicketRazziaInformationEnumeration_s;
@end

enum {
  TicketRazziaInformationEnumeration_razzia = 1,
  TicketRazziaInformationEnumeration_norazzia = 2,
};

extern string_tree_s g_TicketRazziaInformationEnumeration_tree;

WUR int TicketRazziaInformationEnumeration_s_write(TicketRazziaInformationEnumeration_s *this,bc_array_s *a_trg);
WUR int TicketRazziaInformationEnumeration_s_read(TicketRazziaInformationEnumeration_s *this,var_s a_var);

// -- TicketValidationEnumeration_s --
@begin
struct
<
ui:value
>
TicketValidationEnumeration_s;
@end

enum {
  TicketValidationEnumeration_valid = 1,
  TicketValidationEnumeration_notvalid = 2,
  TicketValidationEnumeration_NoCard = 3,
};

extern string_tree_s g_TicketValidationEnumeration_tree;

WUR int TicketValidationEnumeration_s_write(TicketValidationEnumeration_s *this,bc_array_s *a_trg);
WUR int TicketValidationEnumeration_s_read(TicketValidationEnumeration_s *this,var_s a_var);

// -- CountingStateEnumeration_s --
@begin
struct
<
ui:value
>
CountingStateEnumeration_s;
@end

enum {
  CountingStateEnumeration_StartRequested = 1,
  CountingStateEnumeration_Started = 2,
  CountingStateEnumeration_StopRequested = 3,
  CountingStateEnumeration_Stopped = 4,
};

extern string_tree_s g_CountingStateEnumeration_tree;

WUR int CountingStateEnumeration_s_write(CountingStateEnumeration_s *this,bc_array_s *a_trg);
WUR int CountingStateEnumeration_s_read(CountingStateEnumeration_s *this,var_s a_var);

// -- ErrorCodeEnumeration_option_s --
@begin
choice
<
bi:none
ErrorCodeEnumeration_s:value
> ErrorCodeEnumeration_option_s;
@end

// -- ConnectionStateEnumeration_option_s --
@begin
choice
<
bi:none
ConnectionStateEnumeration_s:value
> ConnectionStateEnumeration_option_s;
@end

// -- DoorCountingQualityEnumeration_option_s --
@begin
choice
<
bi:none
DoorCountingQualityEnumeration_s:value
> DoorCountingQualityEnumeration_option_s;
@end

// -- TicketingService_SetRazziaRequestStructure_s --
@begin
struct
<
TicketRazziaInformationEnumeration_s:TicketInformationRazziaStatus
>
TicketingService_SetRazziaRequestStructure_s;
@end

WUR int TicketingService_SetRazziaRequestStructure_s_write(TicketingService_SetRazziaRequestStructure_s *this,bc_array_s *a_trg);
WUR int TicketingService_SetRazziaRequestStructure_s_read(TicketingService_SetRazziaRequestStructure_s *this,var_s a_var);

// -- IBIS_IP_int_s --
@begin
struct
<
xs_int_s:Value
ErrorCodeEnumeration_option_s:ErrorCode $// [0,1]
>
IBIS_IP_int_s;
@end

WUR int IBIS_IP_int_s_write(IBIS_IP_int_s *this,bc_array_s *a_trg);
WUR int IBIS_IP_int_s_read(IBIS_IP_int_s *this,var_s a_var);

// -- IBIS_IP_byte_s --
@begin
struct
<
xs_byte_s:Value
ErrorCodeEnumeration_option_s:ErrorCode $// [0,1]
>
IBIS_IP_byte_s;
@end

WUR int IBIS_IP_byte_s_write(IBIS_IP_byte_s *this,bc_array_s *a_trg);
WUR int IBIS_IP_byte_s_read(IBIS_IP_byte_s *this,var_s a_var);

// -- IBIS_IP_double_s --
@begin
struct
<
xs_double_s:Value
ErrorCodeEnumeration_option_s:ErrorCode $// [0,1]
>
IBIS_IP_double_s;
@end

WUR int IBIS_IP_double_s_write(IBIS_IP_double_s *this,bc_array_s *a_trg);
WUR int IBIS_IP_double_s_read(IBIS_IP_double_s *this,var_s a_var);

// -- IBIS_IP_string_s --
@begin
struct
<
xs_string_s:Value
ErrorCodeEnumeration_option_s:ErrorCode $// [0,1]
>
IBIS_IP_string_s;
@end

WUR int IBIS_IP_string_s_write(IBIS_IP_string_s *this,bc_array_s *a_trg);
WUR int IBIS_IP_string_s_read(IBIS_IP_string_s *this,var_s a_var);

// -- IBIS_IP_NMTOKEN_s --
@begin
struct
<
xs_NMTOKEN_s:Value
ErrorCodeEnumeration_option_s:ErrorCode $// [0,1]
>
IBIS_IP_NMTOKEN_s;
@end

WUR int IBIS_IP_NMTOKEN_s_write(IBIS_IP_NMTOKEN_s *this,bc_array_s *a_trg);
WUR int IBIS_IP_NMTOKEN_s_read(IBIS_IP_NMTOKEN_s *this,var_s a_var);

// -- DoorIdType_s --
@begin
struct
<
xs_NMTOKEN_s:Value
ErrorCodeEnumeration_option_s:ErrorCode $// [0,1]
>
DoorIdType_s;
@end

WUR int DoorIdType_s_write(DoorIdType_s *this,bc_array_s *a_trg);
WUR int DoorIdType_s_read(DoorIdType_s *this,var_s a_var);

// -- IBIS_IP_boolean_s --
@begin
struct
<
xs_boolean_s:Value
ErrorCodeEnumeration_option_s:ErrorCode $// [0,1]
>
IBIS_IP_boolean_s;
@end

WUR int IBIS_IP_boolean_s_write(IBIS_IP_boolean_s *this,bc_array_s *a_trg);
WUR int IBIS_IP_boolean_s_read(IBIS_IP_boolean_s *this,var_s a_var);

// -- IBIS_IP_dateTime_s --
@begin
struct
<
xs_dateTime_s:Value
ErrorCodeEnumeration_option_s:ErrorCode $// [0,1]
>
IBIS_IP_dateTime_s;
@end

WUR int IBIS_IP_dateTime_s_write(IBIS_IP_dateTime_s *this,bc_array_s *a_trg);
WUR int IBIS_IP_dateTime_s_read(IBIS_IP_dateTime_s *this,var_s a_var);

// -- IBIS_IP_duration_s --
@begin
struct
<
xs_duration_s:Value
ErrorCodeEnumeration_option_s:ErrorCode $// [0,1]
>
IBIS_IP_duration_s;
@end

WUR int IBIS_IP_duration_s_write(IBIS_IP_duration_s *this,bc_array_s *a_trg);
WUR int IBIS_IP_duration_s_read(IBIS_IP_duration_s *this,var_s a_var);

// -- IBIS_IP_language_s --
@begin
struct
<
xs_language_s:Value
ErrorCodeEnumeration_option_s:ErrorCode $// [0,1]
>
IBIS_IP_language_s;
@end

WUR int IBIS_IP_language_s_write(IBIS_IP_language_s *this,bc_array_s *a_trg);
WUR int IBIS_IP_language_s_read(IBIS_IP_language_s *this,var_s a_var);

// -- GNSSCoordinateStructure_s --
@begin
struct
<
IBIS_IP_double_s:Degree
IBIS_IP_string_s:Direction
>
GNSSCoordinateStructure_s;
@end

WUR int GNSSCoordinateStructure_s_write(GNSSCoordinateStructure_s *this,bc_array_s *a_trg);
WUR int GNSSCoordinateStructure_s_read(GNSSCoordinateStructure_s *this,var_s a_var);

// -- IBIS_IP_unsignedInt_s --
@begin
struct
<
xs_unsignedInt_s:Value
ErrorCodeEnumeration_option_s:ErrorCode $// [0,1]
>
IBIS_IP_unsignedInt_s;
@end

WUR int IBIS_IP_unsignedInt_s_write(IBIS_IP_unsignedInt_s *this,bc_array_s *a_trg);
WUR int IBIS_IP_unsignedInt_s_read(IBIS_IP_unsignedInt_s *this,var_s a_var);

// -- IBIS_IP_unsignedLong_s --
@begin
struct
<
xs_unsignedLong_s:Value
ErrorCodeEnumeration_option_s:ErrorCode $// [0,1]
>
IBIS_IP_unsignedLong_s;
@end

WUR int IBIS_IP_unsignedLong_s_write(IBIS_IP_unsignedLong_s *this,bc_array_s *a_trg);
WUR int IBIS_IP_unsignedLong_s_read(IBIS_IP_unsignedLong_s *this,var_s a_var);

// -- TicketingService_ValidationResultDataStructure_s --
@begin
struct
<
IBIS_IP_dateTime_s:TimeStamp
TicketValidationEnumeration_s:ValidationResult
>
TicketingService_ValidationResultDataStructure_s;
@end

WUR int TicketingService_ValidationResultDataStructure_s_write(TicketingService_ValidationResultDataStructure_s *this,bc_array_s *a_trg);
WUR int TicketingService_ValidationResultDataStructure_s_read(TicketingService_ValidationResultDataStructure_s *this,var_s a_var);

// -- IBIS_IP_nonNegativeInteger_s --
@begin
struct
<
xs_nonNegativeInteger_s:Value
ErrorCodeEnumeration_option_s:ErrorCode $// [0,1]
>
IBIS_IP_nonNegativeInteger_s;
@end

WUR int IBIS_IP_nonNegativeInteger_s_write(IBIS_IP_nonNegativeInteger_s *this,bc_array_s *a_trg);
WUR int IBIS_IP_nonNegativeInteger_s_read(IBIS_IP_nonNegativeInteger_s *this,var_s a_var);

// -- DoorOpenStateStructure_s --
@begin
struct
<
DoorOpenStateEnumeration_s:Value
ErrorCodeEnumeration_option_s:ErrorCode $// [0,1]
>
DoorOpenStateStructure_s;
@end

WUR int DoorOpenStateStructure_s_write(DoorOpenStateStructure_s *this,bc_array_s *a_trg);
WUR int DoorOpenStateStructure_s_read(DoorOpenStateStructure_s *this,var_s a_var);

// -- DoorOperationStateStructure_s --
@begin
struct
<
DoorOperationStateEnumeration_s:Value
ErrorCodeEnumeration_option_s:ErrorCode $// [0,1]
>
DoorOperationStateStructure_s;
@end

WUR int DoorOperationStateStructure_s_write(DoorOperationStateStructure_s *this,bc_array_s *a_trg);
WUR int DoorOperationStateStructure_s_read(DoorOperationStateStructure_s *this,var_s a_var);

// -- InternationalTextType_s --
@begin
struct
<
xs_string_s:Value
xs_language_s:Language
ErrorCodeEnumeration_option_s:ErrorCode $// [0,1]
>
InternationalTextType_s;
@end

WUR int InternationalTextType_s_write(InternationalTextType_s *this,bc_array_s *a_trg);
WUR int InternationalTextType_s_read(InternationalTextType_s *this,var_s a_var);

// -- DoorCountingStructure_s --
@begin
struct
<
DoorCountingObjectClassEnumeration_s:ObjectClass
IBIS_IP_int_s:In
IBIS_IP_int_s:Out
DoorCountingQualityEnumeration_option_s:CountQuality $// [0,1]
>
DoorCountingStructure_s;
@end

WUR int DoorCountingStructure_s_write(DoorCountingStructure_s *this,bc_array_s *a_trg);
WUR int DoorCountingStructure_s_read(DoorCountingStructure_s *this,var_s a_var);

// -- DoorIdType_array_s --
@begin
array<DoorIdType_s> DoorIdType_array_s;
@end

// -- IBIS_IP_int_option_s --
@begin
choice
<
bi:none
IBIS_IP_int_s:value
> IBIS_IP_int_option_s;
@end

// -- IBIS_IP_byte_array_s --
@begin
array<IBIS_IP_byte_s> IBIS_IP_byte_array_s;
@end

// -- IBIS_IP_double_option_s --
@begin
choice
<
bi:none
IBIS_IP_double_s:value
> IBIS_IP_double_option_s;
@end

// -- IBIS_IP_string_option_s --
@begin
choice
<
bi:none
IBIS_IP_string_s:value
> IBIS_IP_string_option_s;
@end

// -- IBIS_IP_string_array_s --
@begin
array<IBIS_IP_string_s> IBIS_IP_string_array_s;
@end

// -- IBIS_IP_NMTOKEN_option_s --
@begin
choice
<
bi:none
IBIS_IP_NMTOKEN_s:value
> IBIS_IP_NMTOKEN_option_s;
@end

// -- PassengerCountingService_RetrieveSpecificDoorDataRequestStructure_s --
@begin
struct
<
IBIS_IP_NMTOKEN_s:DoorID
>
PassengerCountingService_RetrieveSpecificDoorDataRequestStructure_s;
@end

WUR int PassengerCountingService_RetrieveSpecificDoorDataRequestStructure_s_write(PassengerCountingService_RetrieveSpecificDoorDataRequestStructure_s *this,bc_array_s *a_trg);
WUR int PassengerCountingService_RetrieveSpecificDoorDataRequestStructure_s_read(PassengerCountingService_RetrieveSpecificDoorDataRequestStructure_s *this,var_s a_var);

// -- TicketingService_RetrieveTariffInformationRequestStructure_s --
@begin
struct
<
IBIS_IP_NMTOKEN_s:TripRef
IBIS_IP_NMTOKEN_s:LineRef
IBIS_IP_NMTOKEN_s:StopRef
>
TicketingService_RetrieveTariffInformationRequestStructure_s;
@end

WUR int TicketingService_RetrieveTariffInformationRequestStructure_s_write(TicketingService_RetrieveTariffInformationRequestStructure_s *this,bc_array_s *a_trg);
WUR int TicketingService_RetrieveTariffInformationRequestStructure_s_read(TicketingService_RetrieveTariffInformationRequestStructure_s *this,var_s a_var);

// -- IBIS_IP_boolean_option_s --
@begin
choice
<
bi:none
IBIS_IP_boolean_s:value
> IBIS_IP_boolean_option_s;
@end

// -- DoorIdListType_s --
@begin
struct
<
DoorIdType_array_s:DoorID $// [1,unbounded]
>
DoorIdListType_s;
@end

WUR int DoorIdListType_s_write(DoorIdListType_s *this,bc_array_s *a_trg);
WUR int DoorIdListType_s_read(DoorIdListType_s *this,var_s a_var);

// -- IBIS_IP_dateTime_option_s --
@begin
choice
<
bi:none
IBIS_IP_dateTime_s:value
> IBIS_IP_dateTime_option_s;
@end

// -- IBIS_IP_duration_option_s --
@begin
choice
<
bi:none
IBIS_IP_duration_s:value
> IBIS_IP_duration_option_s;
@end

// -- IBIS_IP_unsignedInt_option_s --
@begin
choice
<
bi:none
IBIS_IP_unsignedInt_s:value
> IBIS_IP_unsignedInt_option_s;
@end

// -- DoorCountingStructure_array_s --
@begin
array<DoorCountingStructure_s> DoorCountingStructure_array_s;
@end

// -- BayAreaStructure_s --
@begin
struct
<
IBIS_IP_double_option_s:BeforeBay $// [0,1]
IBIS_IP_double_option_s:BehindBay $// [0,1]
>
BayAreaStructure_s;
@end

WUR int BayAreaStructure_s_write(BayAreaStructure_s *this,bc_array_s *a_trg);
WUR int BayAreaStructure_s_read(BayAreaStructure_s *this,var_s a_var);

// -- InternationalTextType_array_s --
@begin
array<InternationalTextType_s> InternationalTextType_array_s;
@end

// -- IBIS_IP_nonNegativeInteger_option_s --
@begin
choice
<
bi:none
IBIS_IP_nonNegativeInteger_s:value
> IBIS_IP_nonNegativeInteger_option_s;
@end

// -- DoorOperationStateStructure_option_s --
@begin
choice
<
bi:none
DoorOperationStateStructure_s:value
> DoorOperationStateStructure_option_s;
@end

// -- TicketingService_ValidationResultDataStructure_option_s --
@begin
choice
<
bi:none
TicketingService_ValidationResultDataStructure_s:value
> TicketingService_ValidationResultDataStructure_option_s;
@end

// -- CountingStateStructure_s --
@begin
struct
<
DoorIdType_s:DoorID
CountingStateEnumeration_s:CountingState
>
CountingStateStructure_s;
@end

WUR int CountingStateStructure_s_write(CountingStateStructure_s *this,bc_array_s *a_trg);
WUR int CountingStateStructure_s_read(CountingStateStructure_s *this,var_s a_var);

// -- DoorCountingListStructure_s --
@begin
struct
<
IBIS_IP_NMTOKEN_s:DoorID
DoorCountingStructure_array_s:CountSet $// [1,unbounded]
>
DoorCountingListStructure_s;
@end

WUR int DoorCountingListStructure_s_write(DoorCountingListStructure_s *this,bc_array_s *a_trg);
WUR int DoorCountingListStructure_s_read(DoorCountingListStructure_s *this,var_s a_var);

// -- AnnouncementStructure_s --
@begin
struct
<
IBIS_IP_NMTOKEN_s:AnnouncementRef
InternationalTextType_array_s:AnnouncementText $// [0,unbounded]
InternationalTextType_array_s:AnnouncementTTSText $// [0,unbounded]
>
AnnouncementStructure_s;
@end

WUR int AnnouncementStructure_s_write(AnnouncementStructure_s *this,bc_array_s *a_trg);
WUR int AnnouncementStructure_s_read(AnnouncementStructure_s *this,var_s a_var);

// -- CardType_s --
@begin
struct
<
IBIS_IP_NMTOKEN_s:CardSerialNumber
IBIS_IP_NMTOKEN_s:CardTypeID
InternationalTextType_array_s:CardTypeText $// [0,unbounded]
>
CardType_s;
@end

WUR int CardType_s_write(CardType_s *this,bc_array_s *a_trg);
WUR int CardType_s_read(CardType_s *this,var_s a_var);

// -- DestinationStructure_s --
@begin
struct
<
IBIS_IP_NMTOKEN_s:DestinationRef
InternationalTextType_array_s:DestinationName $// [0,unbounded]
InternationalTextType_array_s:DestinationShortName $// [0,unbounded]
>
DestinationStructure_s;
@end

WUR int DestinationStructure_s_write(DestinationStructure_s *this,bc_array_s *a_trg);
WUR int DestinationStructure_s_read(DestinationStructure_s *this,var_s a_var);

// -- VehicleStructure_s --
@begin
struct
<
IBIS_IP_NMTOKEN_s:VehicleTypeRef
InternationalTextType_array_s:Name $// [0,unbounded]
>
VehicleStructure_s;
@end

WUR int VehicleStructure_s_write(VehicleStructure_s *this,bc_array_s *a_trg);
WUR int VehicleStructure_s_read(VehicleStructure_s *this,var_s a_var);

// -- ZoneType_s --
@begin
struct
<
IBIS_IP_NMTOKEN_s:FarezoneTypeID
InternationalTextType_array_s:FareZoneTypeName $// [0,unbounded]
>
ZoneType_s;
@end

WUR int ZoneType_s_write(ZoneType_s *this,bc_array_s *a_trg);
WUR int ZoneType_s_read(ZoneType_s *this,var_s a_var);

// -- CardApplInformations_s --
@begin
struct
<
IBIS_IP_unsignedInt_s:CardApplInformationLength
IBIS_IP_byte_array_s:CardApplInformationData $// [1,unbounded]
>
CardApplInformations_s;
@end

WUR int CardApplInformations_s_write(CardApplInformations_s *this,bc_array_s *a_trg);
WUR int CardApplInformations_s_read(CardApplInformations_s *this,var_s a_var);

// -- GlobalCardStatus_s --
@begin
struct
<
IBIS_IP_unsignedInt_s:GlobalCardStausID
IBIS_IP_string_array_s:GlobalCardStatusText $// [0,unbounded]
>
GlobalCardStatus_s;
@end

WUR int GlobalCardStatus_s_write(GlobalCardStatus_s *this,bc_array_s *a_trg);
WUR int GlobalCardStatus_s_read(GlobalCardStatus_s *this,var_s a_var);

// -- SubscribeResponseStructure_s --
@begin
struct
<
IBIS_IP_boolean_option_s:Active $// [0,1]
IBIS_IP_string_option_s:OperationErrorMessage $// [0,1]
>
SubscribeResponseStructure_s;
@end

WUR int SubscribeResponseStructure_s_write(SubscribeResponseStructure_s *this,bc_array_s *a_trg);
WUR int SubscribeResponseStructure_s_read(SubscribeResponseStructure_s *this,var_s a_var);

// -- UnsubscribeResponseStructure_s --
@begin
struct
<
IBIS_IP_boolean_option_s:Active $// [0,1]
IBIS_IP_string_option_s:OperationErrorMessage $// [0,1]
>
UnsubscribeResponseStructure_s;
@end

WUR int UnsubscribeResponseStructure_s_write(UnsubscribeResponseStructure_s *this,bc_array_s *a_trg);
WUR int UnsubscribeResponseStructure_s_read(UnsubscribeResponseStructure_s *this,var_s a_var);

// -- TicketingService_GetValidationResultResponseStructure_s --
@begin
struct
<
TicketingService_ValidationResultDataStructure_option_s:ValidationResultData $// [0,1]
IBIS_IP_string_option_s:OperationErrorMessage $// [0,1]
>
TicketingService_GetValidationResultResponseStructure_s;
@end

WUR int TicketingService_GetValidationResultResponseStructure_s_write(TicketingService_GetValidationResultResponseStructure_s *this,bc_array_s *a_trg);
WUR int TicketingService_GetValidationResultResponseStructure_s_read(TicketingService_GetValidationResultResponseStructure_s *this,var_s a_var);

// -- DoorStateStructure_s --
@begin
struct
<
DoorOpenStateStructure_s:OpenState
DoorOperationStateStructure_option_s:OperationState $// [0,1]
>
DoorStateStructure_s;
@end

WUR int DoorStateStructure_s_write(DoorStateStructure_s *this,bc_array_s *a_trg);
WUR int DoorStateStructure_s_read(DoorStateStructure_s *this,var_s a_var);

// -- SubscribeRequestStructure_s --
@begin
struct
<
IBIS_IP_string_s:Client_IP_Address
IBIS_IP_int_option_s:ReplyPort $// [0,1]
IBIS_IP_string_option_s:ReplyPath $// [0,1]
>
SubscribeRequestStructure_s;
@end

WUR int SubscribeRequestStructure_s_write(SubscribeRequestStructure_s *this,bc_array_s *a_trg);
WUR int SubscribeRequestStructure_s_read(SubscribeRequestStructure_s *this,var_s a_var);

// -- UnsubscribeRequestStructure_s --
@begin
struct
<
IBIS_IP_string_s:Client_IP_Address
IBIS_IP_int_option_s:ReplyPort $// [0,1]
IBIS_IP_string_option_s:ReplyPath $// [0,1]
>
UnsubscribeRequestStructure_s;
@end

WUR int UnsubscribeRequestStructure_s_write(UnsubscribeRequestStructure_s *this,bc_array_s *a_trg);
WUR int UnsubscribeRequestStructure_s_read(UnsubscribeRequestStructure_s *this,var_s a_var);

// -- LineInformationStructure_s --
@begin
struct
<
IBIS_IP_NMTOKEN_s:LineRef
InternationalTextType_array_s:LineName $// [0,unbounded]
InternationalTextType_array_s:LineShortName $// [0,unbounded]
IBIS_IP_int_option_s:LineNumber $// [0,1]
>
LineInformationStructure_s;
@end

WUR int LineInformationStructure_s_write(LineInformationStructure_s *this,bc_array_s *a_trg);
WUR int LineInformationStructure_s_read(LineInformationStructure_s *this,var_s a_var);

// -- CardTicketData_s --
@begin
struct
<
IBIS_IP_unsignedLong_s:CardTicketDataID
IBIS_IP_unsignedInt_s:CardTicketDataLength
IBIS_IP_byte_array_s:CardTicketData $// [1,unbounded]
>
CardTicketData_s;
@end

WUR int CardTicketData_s_write(CardTicketData_s *this,bc_array_s *a_trg);
WUR int CardTicketData_s_read(CardTicketData_s *this,var_s a_var);

// -- GNSSPointStructure_s --
@begin
struct
<
IBIS_IP_NMTOKEN_option_s:PointRef $// [0,1]
GNSSCoordinateStructure_s:Longitude
GNSSCoordinateStructure_s:Latitude
IBIS_IP_double_option_s:Altitude $// [0,1]
>
GNSSPointStructure_s;
@end

WUR int GNSSPointStructure_s_write(GNSSPointStructure_s *this,bc_array_s *a_trg);
WUR int GNSSPointStructure_s_read(GNSSPointStructure_s *this,var_s a_var);

// -- ViaPointStructure_s --
@begin
struct
<
IBIS_IP_NMTOKEN_s:ViaPointRef
IBIS_IP_NMTOKEN_option_s:PlaceRef $// [0,1]
InternationalTextType_array_s:PlaceName $// [0,unbounded]
InternationalTextType_array_s:PlaceShortName $// [0,unbounded]
IBIS_IP_int_option_s:ViaPointDisplayPriority $// [0,1]
>
ViaPointStructure_s;
@end

WUR int ViaPointStructure_s_write(ViaPointStructure_s *this,bc_array_s *a_trg);
WUR int ViaPointStructure_s_read(ViaPointStructure_s *this,var_s a_var);

// -- DataAcceptedResponseDataStructure_s --
@begin
struct
<
IBIS_IP_dateTime_s:TimeStamp
IBIS_IP_boolean_s:DataAccepted
ErrorCodeEnumeration_option_s:ErrorCode $// [0,1]
IBIS_IP_string_option_s:ErrorInformation $// [0,1]
>
DataAcceptedResponseDataStructure_s;
@end

WUR int DataAcceptedResponseDataStructure_s_write(DataAcceptedResponseDataStructure_s *this,bc_array_s *a_trg);
WUR int DataAcceptedResponseDataStructure_s_read(DataAcceptedResponseDataStructure_s *this,var_s a_var);

// -- ZoneType_option_s --
@begin
choice
<
bi:none
ZoneType_s:value
> ZoneType_option_s;
@end

// -- PassengerCountingService_StartCountingRequestStructure_s --
@begin
struct
<
DoorIdListType_s:DoorIdList
>
PassengerCountingService_StartCountingRequestStructure_s;
@end

WUR int PassengerCountingService_StartCountingRequestStructure_s_write(PassengerCountingService_StartCountingRequestStructure_s *this,bc_array_s *a_trg);
WUR int PassengerCountingService_StartCountingRequestStructure_s_read(PassengerCountingService_StartCountingRequestStructure_s *this,var_s a_var);

// -- PassengerCountingService_StopCountingRequestStructure_s --
@begin
struct
<
DoorIdListType_s:DoorIdList
>
PassengerCountingService_StopCountingRequestStructure_s;
@end

WUR int PassengerCountingService_StopCountingRequestStructure_s_write(PassengerCountingService_StopCountingRequestStructure_s *this,bc_array_s *a_trg);
WUR int PassengerCountingService_StopCountingRequestStructure_s_read(PassengerCountingService_StopCountingRequestStructure_s *this,var_s a_var);

// -- BayAreaStructure_option_s --
@begin
choice
<
bi:none
BayAreaStructure_s:value
> BayAreaStructure_option_s;
@end

// -- VehicleStructure_option_s --
@begin
choice
<
bi:none
VehicleStructure_s:value
> VehicleStructure_option_s;
@end

// -- ViaPointStructure_array_s --
@begin
array<ViaPointStructure_s> ViaPointStructure_array_s;
@end

// -- DoorStateStructure_option_s --
@begin
choice
<
bi:none
DoorStateStructure_s:value
> DoorStateStructure_option_s;
@end

// -- GNSSPointStructure_option_s --
@begin
choice
<
bi:none
GNSSPointStructure_s:value
> GNSSPointStructure_option_s;
@end

// -- CardApplInformations_option_s --
@begin
choice
<
bi:none
CardApplInformations_s:value
> CardApplInformations_option_s;
@end

// -- AnnouncementStructure_option_s --
@begin
choice
<
bi:none
AnnouncementStructure_s:value
> AnnouncementStructure_option_s;
@end

// -- CountingStateStructure_array_s --
@begin
array<CountingStateStructure_s> CountingStateStructure_array_s;
@end

// -- DoorCountingListStructure_array_s --
@begin
array<DoorCountingListStructure_s> DoorCountingListStructure_array_s;
@end

// -- PassengerCountingService_SetCounterDataRequestStructure_s --
@begin
struct
<
DoorCountingListStructure_array_s:DoorSetList $// [1,unbounded]
>
PassengerCountingService_SetCounterDataRequestStructure_s;
@end

WUR int PassengerCountingService_SetCounterDataRequestStructure_s_write(PassengerCountingService_SetCounterDataRequestStructure_s *this,bc_array_s *a_trg);
WUR int PassengerCountingService_SetCounterDataRequestStructure_s_read(PassengerCountingService_SetCounterDataRequestStructure_s *this,var_s a_var);

// -- DataAcceptedResponseDataStructure_option_s --
@begin
choice
<
bi:none
DataAcceptedResponseDataStructure_s:value
> DataAcceptedResponseDataStructure_option_s;
@end

// -- PassengerCountingService_CountingStatesStructure_s --
@begin
struct
<
IBIS_IP_dateTime_s:TimeStamp
CountingStateStructure_array_s:CountingStates $// [1,unbounded]
>
PassengerCountingService_CountingStatesStructure_s;
@end

WUR int PassengerCountingService_CountingStatesStructure_s_write(PassengerCountingService_CountingStatesStructure_s *this,bc_array_s *a_trg);
WUR int PassengerCountingService_CountingStatesStructure_s_read(PassengerCountingService_CountingStatesStructure_s *this,var_s a_var);

// -- DataAcceptedResponseStructure_s --
@begin
struct
<
DataAcceptedResponseDataStructure_option_s:DataAcceptedResponseData $// [0,1]
IBIS_IP_string_option_s:OperationErrorMessage $// [0,1]
>
DataAcceptedResponseStructure_s;
@end

WUR int DataAcceptedResponseStructure_s_write(DataAcceptedResponseStructure_s *this,bc_array_s *a_trg);
WUR int DataAcceptedResponseStructure_s_read(DataAcceptedResponseStructure_s *this,var_s a_var);

// -- FareZoneInformationStructure_s --
@begin
struct
<
IBIS_IP_NMTOKEN_s:FareZoneID
ZoneType_option_s:FareZoneType $// [0,1]
InternationalTextType_array_s:FareZoneLongName $// [0,unbounded]
InternationalTextType_array_s:FareZoneShortName $// [0,unbounded]
>
FareZoneInformationStructure_s;
@end

WUR int FareZoneInformationStructure_s_write(FareZoneInformationStructure_s *this,bc_array_s *a_trg);
WUR int FareZoneInformationStructure_s_read(FareZoneInformationStructure_s *this,var_s a_var);

// -- DoorInformationStructure_s --
@begin
struct
<
IBIS_IP_NMTOKEN_s:DoorID
DoorCountingStructure_array_s:Count $// [1,unbounded]
DoorStateStructure_option_s:State $// [0,1]
>
DoorInformationStructure_s;
@end

WUR int DoorInformationStructure_s_write(DoorInformationStructure_s *this,bc_array_s *a_trg);
WUR int DoorInformationStructure_s_read(DoorInformationStructure_s *this,var_s a_var);

// -- TicketingService_ValidateTicketRequestStructure_s --
@begin
struct
<
CardType_s:CardType
CardApplInformations_option_s:CardApplInformation $// [0,1]
IBIS_IP_unsignedInt_s:NumberOfCardTicketDataBlocks
CardTicketData_s:CardTicketDataBlock
>
TicketingService_ValidateTicketRequestStructure_s;
@end

WUR int TicketingService_ValidateTicketRequestStructure_s_write(TicketingService_ValidateTicketRequestStructure_s *this,bc_array_s *a_trg);
WUR int TicketingService_ValidateTicketRequestStructure_s_read(TicketingService_ValidateTicketRequestStructure_s *this,var_s a_var);

// -- TicketingService_ValidateTicketResponseDataStructure_s --
@begin
struct
<
IBIS_IP_dateTime_s:TimeStamp
GlobalCardStatus_s:GlobalCardStatus
CardType_s:CardType
IBIS_IP_unsignedInt_option_s:CardApplStatusCode $// [0,1]
CardApplInformations_option_s:CardApplikationInformation $// [0,1]
IBIS_IP_unsignedInt_s:CardValidationCode
CardTicketData_s:CardTicketData
>
TicketingService_ValidateTicketResponseDataStructure_s;
@end

WUR int TicketingService_ValidateTicketResponseDataStructure_s_write(TicketingService_ValidateTicketResponseDataStructure_s *this,bc_array_s *a_trg);
WUR int TicketingService_ValidateTicketResponseDataStructure_s_read(TicketingService_ValidateTicketResponseDataStructure_s *this,var_s a_var);

// -- DisplayContentStructure_s --
@begin
struct
<
IBIS_IP_NMTOKEN_option_s:DisplayContentRef $// [0,1]
LineInformationStructure_s:LineInformation
DestinationStructure_s:Destination
ViaPointStructure_array_s:ViaPoint $// [0,unbounded]
InternationalTextType_array_s:AdditionalInformation $// [0,unbounded]
IBIS_IP_nonNegativeInteger_option_s:Priority $// [0,1]
IBIS_IP_duration_option_s:PeriodDuration $// [0,1]
IBIS_IP_duration_option_s:Duration $// [0,1]
>
DisplayContentStructure_s;
@end

WUR int DisplayContentStructure_s_write(DisplayContentStructure_s *this,bc_array_s *a_trg);
WUR int DisplayContentStructure_s_read(DisplayContentStructure_s *this,var_s a_var);

// -- DisplayContentStructure_array_s --
@begin
array<DisplayContentStructure_s> DisplayContentStructure_array_s;
@end

// -- DoorInformationStructure_array_s --
@begin
array<DoorInformationStructure_s> DoorInformationStructure_array_s;
@end

// -- PassengerCountingService_CountingStatesStructure_option_s --
@begin
choice
<
bi:none
PassengerCountingService_CountingStatesStructure_s:value
> PassengerCountingService_CountingStatesStructure_option_s;
@end

// -- TicketingService_ValidateTicketResponseDataStructure_option_s --
@begin
choice
<
bi:none
TicketingService_ValidateTicketResponseDataStructure_s:value
> TicketingService_ValidateTicketResponseDataStructure_option_s;
@end

// -- PassengerCountingService_SpecificDoorDataStructure_s --
@begin
struct
<
IBIS_IP_dateTime_s:TimeStamp
DoorInformationStructure_s:CountingData
>
PassengerCountingService_SpecificDoorDataStructure_s;
@end

WUR int PassengerCountingService_SpecificDoorDataStructure_s_write(PassengerCountingService_SpecificDoorDataStructure_s *this,bc_array_s *a_trg);
WUR int PassengerCountingService_SpecificDoorDataStructure_s_read(PassengerCountingService_SpecificDoorDataStructure_s *this,var_s a_var);

// -- PassengerCountingService_AllDataStructure_s --
@begin
struct
<
IBIS_IP_dateTime_s:TimeStamp
DoorInformationStructure_array_s:CountingData $// [0,unbounded]
>
PassengerCountingService_AllDataStructure_s;
@end

WUR int PassengerCountingService_AllDataStructure_s_write(PassengerCountingService_AllDataStructure_s *this,bc_array_s *a_trg);
WUR int PassengerCountingService_AllDataStructure_s_read(PassengerCountingService_AllDataStructure_s *this,var_s a_var);

// -- PassengerCountingService_GetCountingStateResponseStructure_s --
@begin
struct
<
PassengerCountingService_CountingStatesStructure_option_s:Data $// [0,1]
IBIS_IP_string_option_s:ErrorMessage $// [0,1]
>
PassengerCountingService_GetCountingStateResponseStructure_s;
@end

WUR int PassengerCountingService_GetCountingStateResponseStructure_s_write(PassengerCountingService_GetCountingStateResponseStructure_s *this,bc_array_s *a_trg);
WUR int PassengerCountingService_GetCountingStateResponseStructure_s_read(PassengerCountingService_GetCountingStateResponseStructure_s *this,var_s a_var);

// -- TicketingService_ValidateTicketResponseStructure_s --
@begin
struct
<
TicketingService_ValidateTicketResponseDataStructure_option_s:TicketingService_ValidationResponseData $// [0,1]
IBIS_IP_string_option_s:OperationErrorMessage $// [0,1]
>
TicketingService_ValidateTicketResponseStructure_s;
@end

WUR int TicketingService_ValidateTicketResponseStructure_s_write(TicketingService_ValidateTicketResponseStructure_s *this,bc_array_s *a_trg);
WUR int TicketingService_ValidateTicketResponseStructure_s_read(TicketingService_ValidateTicketResponseStructure_s *this,var_s a_var);

// -- ConnectionStructure_s --
@begin
struct
<
IBIS_IP_NMTOKEN_s:StopRef
IBIS_IP_NMTOKEN_s:ConnectionRef
ConnectionTypeEnumeration_s:ConnectionType
DisplayContentStructure_s:DisplayContent
IBIS_IP_string_option_s:Platform $// [0,1]
ConnectionStateEnumeration_option_s:ConnectionState $// [0,1]
VehicleStructure_option_s:TransportMode $// [0,1]
IBIS_IP_dateTime_option_s:ExpectedDepatureTime $// [0,1]
>
ConnectionStructure_s;
@end

WUR int ConnectionStructure_s_write(ConnectionStructure_s *this,bc_array_s *a_trg);
WUR int ConnectionStructure_s_read(ConnectionStructure_s *this,var_s a_var);

// -- ConnectionStructure_array_s --
@begin
array<ConnectionStructure_s> ConnectionStructure_array_s;
@end

// -- PassengerCountingService_AllDataStructure_option_s --
@begin
choice
<
bi:none
PassengerCountingService_AllDataStructure_s:value
> PassengerCountingService_AllDataStructure_option_s;
@end

// -- PassengerCountingService_SpecificDoorDataStructure_option_s --
@begin
choice
<
bi:none
PassengerCountingService_SpecificDoorDataStructure_s:value
> PassengerCountingService_SpecificDoorDataStructure_option_s;
@end

// -- PassengerCountingService_GetAllDataResponseStructure_s --
@begin
struct
<
PassengerCountingService_AllDataStructure_option_s:AllData $// [0,1]
IBIS_IP_string_option_s:OperationErrorMessage $// [0,1]
>
PassengerCountingService_GetAllDataResponseStructure_s;
@end

WUR int PassengerCountingService_GetAllDataResponseStructure_s_write(PassengerCountingService_GetAllDataResponseStructure_s *this,bc_array_s *a_trg);
WUR int PassengerCountingService_GetAllDataResponseStructure_s_read(PassengerCountingService_GetAllDataResponseStructure_s *this,var_s a_var);

// -- PassengerCountingService_RetrieveSpecificDoorDataResponseStructure_s --
@begin
struct
<
PassengerCountingService_SpecificDoorDataStructure_option_s:SpecificDoorData $// [0,1]
IBIS_IP_string_option_s:OperationErrorMessage $// [0,1]
>
PassengerCountingService_RetrieveSpecificDoorDataResponseStructure_s;
@end

WUR int PassengerCountingService_RetrieveSpecificDoorDataResponseStructure_s_write(PassengerCountingService_RetrieveSpecificDoorDataResponseStructure_s *this,bc_array_s *a_trg);
WUR int PassengerCountingService_RetrieveSpecificDoorDataResponseStructure_s_read(PassengerCountingService_RetrieveSpecificDoorDataResponseStructure_s *this,var_s a_var);

// -- JourneyStopInformationStructure_s --
@begin
struct
<
IBIS_IP_NMTOKEN_s:StopRef
InternationalTextType_array_s:StopName $// [1,unbounded]
InternationalTextType_array_s:StopAlternativeName $// [0,unbounded]
IBIS_IP_string_option_s:Platform $// [0,1]
DisplayContentStructure_array_s:DisplayContent $// [1,unbounded]
AnnouncementStructure_option_s:Announcement $// [0,1]
IBIS_IP_dateTime_option_s:ArrivalScheduled $// [0,1]
IBIS_IP_dateTime_option_s:DepartureScheduled $// [0,1]
ConnectionStructure_array_s:Connection $// [0,unbounded]
BayAreaStructure_option_s:BayArea $// [0,1]
GNSSPointStructure_option_s:GNSSPoint $// [0,1]
IBIS_IP_NMTOKEN_option_s:FareZone $// [0,1]
>
JourneyStopInformationStructure_s;
@end

WUR int JourneyStopInformationStructure_s_write(JourneyStopInformationStructure_s *this,bc_array_s *a_trg);
WUR int JourneyStopInformationStructure_s_read(JourneyStopInformationStructure_s *this,var_s a_var);

// -- ShortTripStopStructure_s --
@begin
struct
<
JourneyStopInformationStructure_s:JourneyStopInformation
FareZoneInformationStructure_s:FareZoneInformation
>
ShortTripStopStructure_s;
@end

WUR int ShortTripStopStructure_s_write(ShortTripStopStructure_s *this,bc_array_s *a_trg);
WUR int ShortTripStopStructure_s_read(ShortTripStopStructure_s *this,var_s a_var);

// -- StopPointTariffInformationStructure_s --
@begin
struct
<
JourneyStopInformationStructure_s:JourneyStopInformation
FareZoneInformationStructure_s:FareZoneInformation
>
StopPointTariffInformationStructure_s;
@end

WUR int StopPointTariffInformationStructure_s_write(StopPointTariffInformationStructure_s *this,bc_array_s *a_trg);
WUR int StopPointTariffInformationStructure_s_read(StopPointTariffInformationStructure_s *this,var_s a_var);

// -- ShortTripStopStructure_array_s --
@begin
array<ShortTripStopStructure_s> ShortTripStopStructure_array_s;
@end

// -- ShortTripStopListStructure_s --
@begin
struct
<
ShortTripStopStructure_array_s:ShortTripStop $// [1,unbounded]
>
ShortTripStopListStructure_s;
@end

WUR int ShortTripStopListStructure_s_write(ShortTripStopListStructure_s *this,bc_array_s *a_trg);
WUR int ShortTripStopListStructure_s_read(ShortTripStopListStructure_s *this,var_s a_var);

// -- TicketingService_GetTariffInformationResponseDataStructure_s --
@begin
struct
<
IBIS_IP_language_s:DefaultLanguage
IBIS_IP_dateTime_s:TimeStamp
IBIS_IP_NMTOKEN_s:TripRef
LineInformationStructure_s:Line
StopPointTariffInformationStructure_s:StopPointTariffInformation
ShortTripStopListStructure_s:ShortTripStopList
>
TicketingService_GetTariffInformationResponseDataStructure_s;
@end

WUR int TicketingService_GetTariffInformationResponseDataStructure_s_write(TicketingService_GetTariffInformationResponseDataStructure_s *this,bc_array_s *a_trg);
WUR int TicketingService_GetTariffInformationResponseDataStructure_s_read(TicketingService_GetTariffInformationResponseDataStructure_s *this,var_s a_var);

// -- TicketingService_GetTariffInformationResponseDataStructure_option_s --
@begin
choice
<
bi:none
TicketingService_GetTariffInformationResponseDataStructure_s:value
> TicketingService_GetTariffInformationResponseDataStructure_option_s;
@end

// -- TicketingService_GetTariffInformationResponseStructure_s --
@begin
struct
<
TicketingService_GetTariffInformationResponseDataStructure_option_s:TicketingService_GetTariffInformationResponseData $// [0,1]
IBIS_IP_string_option_s:OperationErrorMessage $// [0,1]
>
TicketingService_GetTariffInformationResponseStructure_s;
@end

WUR int TicketingService_GetTariffInformationResponseStructure_s_write(TicketingService_GetTariffInformationResponseStructure_s *this,bc_array_s *a_trg);
WUR int TicketingService_GetTariffInformationResponseStructure_s_read(TicketingService_GetTariffInformationResponseStructure_s *this,var_s a_var);

void libvdv301_cll_init();
void libvdv301_cll_clear();

// - vdv301 operations -
enum
{/*{{{*/
  c_vdv301_oper_undefined = 0,
};/*}}}*/

// VDV301 GENERATED DECLARATIONS

// === inline methods of generated structures ==================================

// VDV301 GENERATED INLINES

// -- ConnectionStateEnumeration_s --
@begin
inlines ConnectionStateEnumeration_s
@end

// -- ConnectionTypeEnumeration_s --
@begin
inlines ConnectionTypeEnumeration_s
@end

// -- DoorCountingObjectClassEnumeration_s --
@begin
inlines DoorCountingObjectClassEnumeration_s
@end

// -- DoorCountingQualityEnumeration_s --
@begin
inlines DoorCountingQualityEnumeration_s
@end

// -- DoorOpenStateEnumeration_s --
@begin
inlines DoorOpenStateEnumeration_s
@end

// -- DoorOperationStateEnumeration_s --
@begin
inlines DoorOperationStateEnumeration_s
@end

// -- ErrorCodeEnumeration_s --
@begin
inlines ErrorCodeEnumeration_s
@end

// -- TicketRazziaInformationEnumeration_s --
@begin
inlines TicketRazziaInformationEnumeration_s
@end

// -- TicketValidationEnumeration_s --
@begin
inlines TicketValidationEnumeration_s
@end

// -- CountingStateEnumeration_s --
@begin
inlines CountingStateEnumeration_s
@end

// -- ErrorCodeEnumeration_option_s --
@begin
inlines ErrorCodeEnumeration_option_s
@end

// -- ConnectionStateEnumeration_option_s --
@begin
inlines ConnectionStateEnumeration_option_s
@end

// -- DoorCountingQualityEnumeration_option_s --
@begin
inlines DoorCountingQualityEnumeration_option_s
@end

// -- TicketingService_SetRazziaRequestStructure_s --
@begin
inlines TicketingService_SetRazziaRequestStructure_s
@end

// -- IBIS_IP_int_s --
@begin
inlines IBIS_IP_int_s
@end

// -- IBIS_IP_byte_s --
@begin
inlines IBIS_IP_byte_s
@end

// -- IBIS_IP_double_s --
@begin
inlines IBIS_IP_double_s
@end

// -- IBIS_IP_string_s --
@begin
inlines IBIS_IP_string_s
@end

// -- IBIS_IP_NMTOKEN_s --
@begin
inlines IBIS_IP_NMTOKEN_s
@end

// -- DoorIdType_s --
@begin
inlines DoorIdType_s
@end

// -- IBIS_IP_boolean_s --
@begin
inlines IBIS_IP_boolean_s
@end

// -- IBIS_IP_dateTime_s --
@begin
inlines IBIS_IP_dateTime_s
@end

// -- IBIS_IP_duration_s --
@begin
inlines IBIS_IP_duration_s
@end

// -- IBIS_IP_language_s --
@begin
inlines IBIS_IP_language_s
@end

// -- GNSSCoordinateStructure_s --
@begin
inlines GNSSCoordinateStructure_s
@end

// -- IBIS_IP_unsignedInt_s --
@begin
inlines IBIS_IP_unsignedInt_s
@end

// -- IBIS_IP_unsignedLong_s --
@begin
inlines IBIS_IP_unsignedLong_s
@end

// -- TicketingService_ValidationResultDataStructure_s --
@begin
inlines TicketingService_ValidationResultDataStructure_s
@end

// -- IBIS_IP_nonNegativeInteger_s --
@begin
inlines IBIS_IP_nonNegativeInteger_s
@end

// -- DoorOpenStateStructure_s --
@begin
inlines DoorOpenStateStructure_s
@end

// -- DoorOperationStateStructure_s --
@begin
inlines DoorOperationStateStructure_s
@end

// -- InternationalTextType_s --
@begin
inlines InternationalTextType_s
@end

// -- DoorCountingStructure_s --
@begin
inlines DoorCountingStructure_s
@end

// -- DoorIdType_array_s --
@begin
inlines DoorIdType_array_s
@end

// -- IBIS_IP_int_option_s --
@begin
inlines IBIS_IP_int_option_s
@end

// -- IBIS_IP_byte_array_s --
@begin
inlines IBIS_IP_byte_array_s
@end

// -- IBIS_IP_double_option_s --
@begin
inlines IBIS_IP_double_option_s
@end

// -- IBIS_IP_string_option_s --
@begin
inlines IBIS_IP_string_option_s
@end

// -- IBIS_IP_string_array_s --
@begin
inlines IBIS_IP_string_array_s
@end

// -- IBIS_IP_NMTOKEN_option_s --
@begin
inlines IBIS_IP_NMTOKEN_option_s
@end

// -- PassengerCountingService_RetrieveSpecificDoorDataRequestStructure_s --
@begin
inlines PassengerCountingService_RetrieveSpecificDoorDataRequestStructure_s
@end

// -- TicketingService_RetrieveTariffInformationRequestStructure_s --
@begin
inlines TicketingService_RetrieveTariffInformationRequestStructure_s
@end

// -- IBIS_IP_boolean_option_s --
@begin
inlines IBIS_IP_boolean_option_s
@end

// -- DoorIdListType_s --
@begin
inlines DoorIdListType_s
@end

// -- IBIS_IP_dateTime_option_s --
@begin
inlines IBIS_IP_dateTime_option_s
@end

// -- IBIS_IP_duration_option_s --
@begin
inlines IBIS_IP_duration_option_s
@end

// -- IBIS_IP_unsignedInt_option_s --
@begin
inlines IBIS_IP_unsignedInt_option_s
@end

// -- DoorCountingStructure_array_s --
@begin
inlines DoorCountingStructure_array_s
@end

// -- BayAreaStructure_s --
@begin
inlines BayAreaStructure_s
@end

// -- InternationalTextType_array_s --
@begin
inlines InternationalTextType_array_s
@end

// -- IBIS_IP_nonNegativeInteger_option_s --
@begin
inlines IBIS_IP_nonNegativeInteger_option_s
@end

// -- DoorOperationStateStructure_option_s --
@begin
inlines DoorOperationStateStructure_option_s
@end

// -- TicketingService_ValidationResultDataStructure_option_s --
@begin
inlines TicketingService_ValidationResultDataStructure_option_s
@end

// -- CountingStateStructure_s --
@begin
inlines CountingStateStructure_s
@end

// -- DoorCountingListStructure_s --
@begin
inlines DoorCountingListStructure_s
@end

// -- AnnouncementStructure_s --
@begin
inlines AnnouncementStructure_s
@end

// -- CardType_s --
@begin
inlines CardType_s
@end

// -- DestinationStructure_s --
@begin
inlines DestinationStructure_s
@end

// -- VehicleStructure_s --
@begin
inlines VehicleStructure_s
@end

// -- ZoneType_s --
@begin
inlines ZoneType_s
@end

// -- CardApplInformations_s --
@begin
inlines CardApplInformations_s
@end

// -- GlobalCardStatus_s --
@begin
inlines GlobalCardStatus_s
@end

// -- SubscribeResponseStructure_s --
@begin
inlines SubscribeResponseStructure_s
@end

// -- UnsubscribeResponseStructure_s --
@begin
inlines UnsubscribeResponseStructure_s
@end

// -- TicketingService_GetValidationResultResponseStructure_s --
@begin
inlines TicketingService_GetValidationResultResponseStructure_s
@end

// -- DoorStateStructure_s --
@begin
inlines DoorStateStructure_s
@end

// -- SubscribeRequestStructure_s --
@begin
inlines SubscribeRequestStructure_s
@end

// -- UnsubscribeRequestStructure_s --
@begin
inlines UnsubscribeRequestStructure_s
@end

// -- LineInformationStructure_s --
@begin
inlines LineInformationStructure_s
@end

// -- CardTicketData_s --
@begin
inlines CardTicketData_s
@end

// -- GNSSPointStructure_s --
@begin
inlines GNSSPointStructure_s
@end

// -- ViaPointStructure_s --
@begin
inlines ViaPointStructure_s
@end

// -- DataAcceptedResponseDataStructure_s --
@begin
inlines DataAcceptedResponseDataStructure_s
@end

// -- ZoneType_option_s --
@begin
inlines ZoneType_option_s
@end

// -- PassengerCountingService_StartCountingRequestStructure_s --
@begin
inlines PassengerCountingService_StartCountingRequestStructure_s
@end

// -- PassengerCountingService_StopCountingRequestStructure_s --
@begin
inlines PassengerCountingService_StopCountingRequestStructure_s
@end

// -- BayAreaStructure_option_s --
@begin
inlines BayAreaStructure_option_s
@end

// -- VehicleStructure_option_s --
@begin
inlines VehicleStructure_option_s
@end

// -- ViaPointStructure_array_s --
@begin
inlines ViaPointStructure_array_s
@end

// -- DoorStateStructure_option_s --
@begin
inlines DoorStateStructure_option_s
@end

// -- GNSSPointStructure_option_s --
@begin
inlines GNSSPointStructure_option_s
@end

// -- CardApplInformations_option_s --
@begin
inlines CardApplInformations_option_s
@end

// -- AnnouncementStructure_option_s --
@begin
inlines AnnouncementStructure_option_s
@end

// -- CountingStateStructure_array_s --
@begin
inlines CountingStateStructure_array_s
@end

// -- DoorCountingListStructure_array_s --
@begin
inlines DoorCountingListStructure_array_s
@end

// -- PassengerCountingService_SetCounterDataRequestStructure_s --
@begin
inlines PassengerCountingService_SetCounterDataRequestStructure_s
@end

// -- DataAcceptedResponseDataStructure_option_s --
@begin
inlines DataAcceptedResponseDataStructure_option_s
@end

// -- PassengerCountingService_CountingStatesStructure_s --
@begin
inlines PassengerCountingService_CountingStatesStructure_s
@end

// -- DataAcceptedResponseStructure_s --
@begin
inlines DataAcceptedResponseStructure_s
@end

// -- FareZoneInformationStructure_s --
@begin
inlines FareZoneInformationStructure_s
@end

// -- DoorInformationStructure_s --
@begin
inlines DoorInformationStructure_s
@end

// -- TicketingService_ValidateTicketRequestStructure_s --
@begin
inlines TicketingService_ValidateTicketRequestStructure_s
@end

// -- TicketingService_ValidateTicketResponseDataStructure_s --
@begin
inlines TicketingService_ValidateTicketResponseDataStructure_s
@end

// -- DisplayContentStructure_s --
@begin
inlines DisplayContentStructure_s
@end

// -- DisplayContentStructure_array_s --
@begin
inlines DisplayContentStructure_array_s
@end

// -- DoorInformationStructure_array_s --
@begin
inlines DoorInformationStructure_array_s
@end

// -- PassengerCountingService_CountingStatesStructure_option_s --
@begin
inlines PassengerCountingService_CountingStatesStructure_option_s
@end

// -- TicketingService_ValidateTicketResponseDataStructure_option_s --
@begin
inlines TicketingService_ValidateTicketResponseDataStructure_option_s
@end

// -- PassengerCountingService_SpecificDoorDataStructure_s --
@begin
inlines PassengerCountingService_SpecificDoorDataStructure_s
@end

// -- PassengerCountingService_AllDataStructure_s --
@begin
inlines PassengerCountingService_AllDataStructure_s
@end

// -- PassengerCountingService_GetCountingStateResponseStructure_s --
@begin
inlines PassengerCountingService_GetCountingStateResponseStructure_s
@end

// -- TicketingService_ValidateTicketResponseStructure_s --
@begin
inlines TicketingService_ValidateTicketResponseStructure_s
@end

// -- ConnectionStructure_s --
@begin
inlines ConnectionStructure_s
@end

// -- ConnectionStructure_array_s --
@begin
inlines ConnectionStructure_array_s
@end

// -- PassengerCountingService_AllDataStructure_option_s --
@begin
inlines PassengerCountingService_AllDataStructure_option_s
@end

// -- PassengerCountingService_SpecificDoorDataStructure_option_s --
@begin
inlines PassengerCountingService_SpecificDoorDataStructure_option_s
@end

// -- PassengerCountingService_GetAllDataResponseStructure_s --
@begin
inlines PassengerCountingService_GetAllDataResponseStructure_s
@end

// -- PassengerCountingService_RetrieveSpecificDoorDataResponseStructure_s --
@begin
inlines PassengerCountingService_RetrieveSpecificDoorDataResponseStructure_s
@end

// -- JourneyStopInformationStructure_s --
@begin
inlines JourneyStopInformationStructure_s
@end

// -- ShortTripStopStructure_s --
@begin
inlines ShortTripStopStructure_s
@end

// -- StopPointTariffInformationStructure_s --
@begin
inlines StopPointTariffInformationStructure_s
@end

// -- ShortTripStopStructure_array_s --
@begin
inlines ShortTripStopStructure_array_s
@end

// -- ShortTripStopListStructure_s --
@begin
inlines ShortTripStopListStructure_s
@end

// -- TicketingService_GetTariffInformationResponseDataStructure_s --
@begin
inlines TicketingService_GetTariffInformationResponseDataStructure_s
@end

// -- TicketingService_GetTariffInformationResponseDataStructure_option_s --
@begin
inlines TicketingService_GetTariffInformationResponseDataStructure_option_s
@end

// -- TicketingService_GetTariffInformationResponseStructure_s --
@begin
inlines TicketingService_GetTariffInformationResponseStructure_s
@end

// VDV301 GENERATED INLINES

#endif

