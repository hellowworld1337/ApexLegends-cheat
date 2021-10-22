class RecvProp {
public:
	BYTE Stub[4];
	DWORD Offset;
	BYTE Stub2[0x18];
	class RecvTable* DataTable;
	char* Name;
};

class RecvTable {
public:
	BYTE Stub[8];
	RecvProp** Props;
	int NumProps;
	BYTE Stub2[0x4B4];
	char* Name;
};

class ClientClass {
public:
	BYTE Stub[0x18];
	RecvTable* Table;
	ClientClass* NextClass;
};

DWORD ProcessTable(RecvTable* Table, const char* TableName, const char* PropName, bool* DirOff = nullptr)
{
	MUTATE

	DWORD Offset = 0;
	for (int i = 0; (Table && (i < Table->NumProps)); ++i)
	{
		RecvProp* Prop = Table->Props[i]; if (!Prop) continue;
		if (Prop->DataTable && (Prop->DataTable->NumProps > 0)) {
			bool DirOff = false;
			DWORD Off = ProcessTable(Prop->DataTable, TableName, PropName, &DirOff);
			if (!DirOff && Off) Offset += (Prop->Offset + Off);
			else if (DirOff) return Off;
		}

		if (FC::StrCmp(PropName, Prop->Name, true)) {
			if (FC::StrCmp(TableName, Table->Name, true)) {
				if (DirOff) *DirOff = true; 
				return Prop->Offset;
			} return (Prop->Offset + Offset);
		}	
	}
	
	MUTATE_END
	return Offset;
}

DWORD GetOffset(const char* TableName, const char* Prop)
{
	MUTATE

	//typedef DWORD64(*GetAllClassesFn)();
	static DWORD64 dwStart = 0;// VCall<DWORD64>(EPtr(I::CHLClient), 12);

	if (!dwStart) {
		dwStart = *(DWORD64*)RVA(FindPattern(E("4C 8B 1D ? ? ? ? 4D 85 DB 74 19")), 7);
	}

	DWORD Offset = 0;
	for (ClientClass* Class = (ClientClass*)dwStart; Class != nullptr; Class = Class->NextClass)  {
		if ((Offset = ProcessTable(Class->Table, TableName, Prop))) break;
	} 
	
	MUTATE_END
	return Offset;
}