
#include "StartupManager.h"
#include "PseudoCode.h"
#include "HighResolutionTimer.h"
#include "BasicConsole.h"
#include "MicroSeconds.h"
#include "MathExpressionParser.h"
#include "ConfigurationDatabase.h"
#include "StreamMemoryReference.h"
#include "RegularExpression.h"


using namespace MARTe;


#if 0
/**
 *
 */
struct LexicalAnalyzerRule{
	/**
	 *
	 */
	CCString  			rule;

	/**
	 *
	 */
	CCString 			ruleName;

	/**
	 *
	 */
	uint32 				ruleId;

	/**
	 *
	 */
	bool 				skip;
};
#endif

namespace ruleSet{

const uint32 commentElement    = 1;

const uint32 spaceElement      = 2;

const uint32 identifierElement = 3;

const uint32 stringElement     = 4;

const uint32 numberElement     = 5;

const uint32 operators         = 6;


/*
 */
const RegularExpression::PatternInformation rules[]={
	    {"//!*[^\n]\n"                                               ,"line comment"     , commentElement       ,true},
	    {"/\\*!*?\\a\\*/"                                            ,"multiline comment", commentElement       ,true},
	    {"+[ \n\t,;]"                                                ,"separator"        , spaceElement         ,true},
	    {"&&"                                                        ,"AND operator"     , operators            ,false},
	    {"\\|\\|"                                                    ,"AND operator"     , operators            ,false},
	    {"\\^"                                                       ,"AND operator"     , operators            ,false},
	    {"\\+="                                                      ,"AND operator"     , operators            ,false},
	    {"\\-="                                                      ,"AND operator"     , operators            ,false},
	    {"<="                                                        ,"AND operator"     , operators            ,false},
	    {">="                                                        ,"AND operator"     , operators            ,false},
	    {"\\+"                                                       ,"AND operator"     , operators            ,false},
	    {"\\-"                                                       ,"AND operator"     , operators            ,false},
	    {"\\*"                                                       ,"AND operator"     , operators            ,false},
	    {"/"                                                         ,"AND operator"     , operators            ,false},
	    {"="                                                         ,"AND operator"     , operators            ,false},
	    {"\\("                                                       ,"AND operator"     , operators            ,false},
	    {"\\)"                                                       ,"AND operator"     , operators            ,false},
	    {"[\\w_]*[\\d\\w_]"                                          ,"identifier"	     , identifierElement    ,false},
		{"\"*[^\"]\""                                                ,"string"		     , stringElement		,false},
	    {"(+\\d?(.*\\d)|.*\\d)?([eE]!?[+\\-]{1,5}\\d)"               ,"number"		     , numberElement        ,false},
		RegularExpression::emptyPattern
//		{emptyString												 ,emptyString	     , 0					,false}
};

}



CCString Database=
		"expression = \"X=Z,Y=B \n X=5+B*(float)C*!(X-Y)+PIPPO(X+Y)+Z; \" "
		"RPN = \"READ X \" "
		"X = (int32) 3";


CCString MATHExpr= "X=Z,Y=B \n X=5+B*(float)C*!(X-Y)+PIPPO(X+Y)+Z \n";
//CCString MATHExpr= "X=Z,Y=B,";

CCString RPNCode=
		"READ A\n"
		"READ B\n"
		"SUB\n"
		"CONST float32 3.140000\n"
		"MUL\n"
		"DUP\n"
		"WRITE C\n"
		"CONST float32 0.500000\n"
		"POW\n"
		"TAN\n"
		"CAST int32\n"
		"DUP\n"
		"WRITE D\n"
		"READ C\n"
		"LOG\n"
		"CAST int32\n"
		"GT\n"
		"WRITE E\n"
		"READ N1\n"
		"READ N2\n"
		"SUB\n"
		"DUP\n"
		"READ N3\n"
		"READ N4\n"
		"DIV\n"
		"ADD\n"
		"DUP\n"
		"WRITE N5\n"
		"CAST int64\n"
		"GT\n"
		"WRITE F\n"
;

#if 0
const LexicalAnalyzerRule *Parse(CCString &line,DynamicCString &content){
    int ruleNo = 0;
    ErrorManagement::ErrorType match;
    while((ruleSet::rules[ruleNo].rule.GetSize()> 0) && match){
        CCString pattern = ruleSet::rules[ruleNo].rule;
        CCString lineSave = line;
        DynamicCString matched;
        CStringTool matchedT = matched();
        match = RegularExpression::Match(line,pattern,matchedT);
//        int size = line-lineSave;
        if (match){
//        	content().SetSize(0);
//        	content().Append(lineSave,size);
        	content = matched;
//printf("matched %s %i\n",pattern.GetList(),ruleNo);
        	return &ruleSet::rules[ruleNo];
        } else {
        	match.comparisonFailure = false; // reset this error to allow continuation
        	if (match.outOfRange){
printf("error reading input\n");
        	}
        	if (match.notCompleted){
printf("error not completed\n");
        	}
        	if (match.syntaxError){
printf("syntaxError\n");
        	}

            line = lineSave;
        }
        ruleNo++;
    }
    return NULL;
}

const LexicalAnalyzerRule *Parse2(StreamI &line,DynamicCString &content){
    int ruleNo = 0;
    ErrorManagement::ErrorType match;
    while((ruleSet::rules[ruleNo].rule.GetSize()> 0) && match){
        CCString pattern = ruleSet::rules[ruleNo].rule;
        uint64 position = line.Position();
        DynamicCString matched;
        CStringTool matchedT = matched();
        match = RegularExpression::Match(line,pattern,matchedT);
        if (match){
        	content = matched;
        	return &ruleSet::rules[ruleNo];
        } else {
        	match.comparisonFailure = false; // reset this error to allow continuation
        	if (match.outOfRange){
printf("error reading input\n");
        	}
        	if (match.notCompleted){
printf("error not completed\n");
        	}
        	if (match.syntaxError){
printf("syntaxError\n");
        	}

            line.Seek(position);
        }
        ruleNo++;
    }
    return NULL;
}
#endif

CCString line =
" 121 ALPHA \"BIRRA\" // pip\tpo\n"
" 122.5 _ALPha   \"BIRRA\"\"\" // pippo\n"
"/* 123 ALPHA \"BIRRA\" // pippo\n"
" 124 ALPHA \"BIRRA\" // */ ;pippo\n"
" .124 126E4 .81 12.7E-5 231.32E97 .165E3 \n"
" (A + B) * C >= (D+X)/ 6\n";

int main(){

	StartupManager::Initialise();

//char *p = const_cast<char *>(line.GetList());

#if 1
{
		CCString lineP = line;
		ErrorManagement::ErrorType ret;
		while ((lineP[0]!= 0) && ret) {
			DynamicCString content;
		//	const LexicalAnalyzerRule *q = Parse(lineP,content);
			const RegularExpression::PatternInformation *selectedRule = NULL;
			ret = RegularExpression::MatchRules(lineP,ZeroTerminatedArray<const RegularExpression::PatternInformation>(&ruleSet::rules[0]),selectedRule,content);

			if (selectedRule){
				if (!selectedRule->skip){
					printf("%s [%s]\n",selectedRule->ruleName.GetList(),content.GetList());
					fflush(stdout);
				}
			} else {
				printf("UNMATCHED %s\n",lineP.GetList());
				break;
			}
		}
}
{
		ErrorManagement::ErrorType ret;
		StreamString lineS = line;
		lineS.Seek(0);
		while (ret) {
			DynamicCString content;
		//	const LexicalAnalyzerRule *q = Parse(lineP,content);
			const RegularExpression::PatternInformation *selectedRule = NULL;
			ret = RegularExpression::MatchRules(lineS,ZeroTerminatedArray<const RegularExpression::PatternInformation>(&ruleSet::rules[0]),selectedRule,content);

			if (selectedRule){
				if (!selectedRule->skip){
					printf("%s [%s]\n",selectedRule->ruleName.GetList(),content.GetList());
					fflush(stdout);
				}
			} else {
				printf("UNMATCHED \n");
				break;
			}
		}

}
//oo

return 0;


	ConfigurationDatabase cdb;

	AnyType at(Database);

	at.SetFormattedStreamType("CDB");
	at.CopyTo(cdb);

return 0;




	StreamString err;
	StreamMemoryReference smr(MATHExpr.GetList(),MATHExpr.GetSize());
	MathExpressionParser mexp;
	mexp.Parse(smr,&err);

	printf(">>\n%s\n<<\n",mexp.GetStackMachineExpression().GetList());
	printf(">>\n%s\n<<\n",err.Buffer().GetList());

	return 0;

#endif


	PseudoCode::Context context;

	ErrorManagement::ErrorType ret;

	ret = context.ExtractVariables(RPNCode);

	if (ret){
		uint32 index = 0U;
		PseudoCode::VariableInformation *var;

		printf ("VAR SCAN RESULT\n");
		while(context.BrowseInputVariable(index,var)){
			printf ("input  var %2i @%04x = %s \n",index,var->location,var->name.GetList());
			index++;
			if (var->name == "A"){
				var->type = TypeDescriptor("float32");
			}
			if (var->name == "B"){
				var->type = TypeDescriptor("float32");
			}
			if (var->name == "N1"){
				var->type = TypeDescriptor("int8");
			}
			if (var->name == "N2"){
				var->type = TypeDescriptor("int8");
			}
			if (var->name == "N3"){
				var->type = TypeDescriptor("int8");
			}
			if (var->name == "N4"){
				var->type = TypeDescriptor("int8");
			}
		}

		index = 0;
		while(context.BrowseOutputVariable(index,var)){
			printf ("output var %2i @%04x = %s\n",index,var->location,var->name.GetList());
			index++;
			if (var->name == "C"){
				var->type = TypeDescriptor("float32");
			}
			if (var->name == "D"){
				var->type = TypeDescriptor("int32");
			}
			if (var->name == "E"){
				var->type = TypeDescriptor("uint8");
			}
			if (var->name == "F"){
				var->type = TypeDescriptor("uint8");
			}
			if (var->name == "N5"){
				var->type = TypeDescriptor("int8");
			}
		}
	}

	if (ret){
		printf ("COMPILE\n");
		ret = context.Compile(RPNCode);
	}

	if (ret){
		printf("SUCCESSFUL\n");
		printf("size of constant area = %i\n",context.startOfVariables);
		printf("size of data area = %i\n",context.dataMemory.GetNumberOfElements());
		printf("size of code area = %i\n",context.codeMemory.GetSize());
		printf("size of stack area = %i\n",context.stack.GetNumberOfElements());
	}

	if (ret){
		printf ("ASSIGN INPUTS\n");
		int32 index = 0;
		PseudoCode::VariableInformation *var;

		while(context.BrowseInputVariable(index,var)){
			index++;
			if (var->name == "A"){
				float *x  = reinterpret_cast<float *>(&context.dataMemory[var->location]);
				*x = 1.0;
			}
			if (var->name == "B"){
				float *x  = reinterpret_cast<float *>(&context.dataMemory[var->location]);
				*x = 2.0;
			}
			if (var->name == "N1"){
				int8 *x  = reinterpret_cast<int8 *>(&context.dataMemory[var->location]);
				*x = 31;
			}
			if (var->name == "N2"){
				int8 *x  = reinterpret_cast<int8 *>(&context.dataMemory[var->location]);
				*x = 22;
			}
			if (var->name == "N3"){
				int8 *x  = reinterpret_cast<int8 *>(&context.dataMemory[var->location]);
				*x = 13;
			}
			if (var->name == "N4"){
				int8 *x  = reinterpret_cast<int8 *>(&context.dataMemory[var->location]);
				*x = 127;
			}
		}
	}

	if (ret){
		printf ("VAR ALLOCATION RESULT\n");
		uint32 index = 0;
		PseudoCode::VariableInformation *var;

		while(context.BrowseInputVariable(index,var)){
			printf ("input  var %2i @%04x = %s \n",index,var->location,var->name.GetList());
			index++;
		}

		index = 0;
		while(context.BrowseOutputVariable(index,var)){
			printf ("output var %2i @%04x = %s\n",index,var->location,var->name.GetList());
			index++;
		}
	}

	if (ret){
		printf ("DECOMPILE showing types pCode\n");
		DynamicCString RPNCodeRev;
		ret = context.DeCompile(RPNCodeRev,true);

		printf("Decompiled:\n%s\n",RPNCodeRev.GetList());
	}

	if (ret){
		printf ("DECOMPILE not showing types pCode\n");
		DynamicCString RPNCodeRev;
		ret = context.DeCompile(RPNCodeRev,false);

		printf("Decompiled:\n%s\n",RPNCodeRev.GetList());
		if (RPNCode == RPNCodeRev){
			printf("identical to the original source\n");
		} else {
			printf("not identical to the original source\n");
		}
	}


	if (ret){
		printf ("DEBUG MODE EXECUTION \n");
		// let stdout catch up..
		fflush(stdout);
		DynamicCString dcs;
		CStringTool cst = dcs();
//		BasicConsole console;
		ret = context.Execute(PseudoCode::Context::debugMode,&cst,0);
		if (dcs.GetSize() > 0){
			printf("%s\n",dcs.GetList());
		}
	}

	//for (int kk = 0;(kk<1000) && ret;kk++)
	if (ret){
		printf ("FAST MODE EXECUTION \n");
		printf ("Executes 1 Million times ");
		Ticks t1,t2;
		t1 = HighResolutionTimer::GetTicks();
		if (ret){
			for (int j = 0;(j<10) && ret;j++) {
				printf (".");
				fflush(stdout);
				for (int i = 0;(i<100000) && ret;i++) {
					ret = context.Execute(PseudoCode::Context::fastMode,NULL,0);
				}
			}
		}
		t2 = HighResolutionTimer::GetTicks();
		MilliSeconds dT = t2-t1;
		printf (" in %i milliseconds\n",dT.GetTimeRaw());
		fflush(stdout);
	}

	if (ret){
		printf ("SAFE MODE EXECUTION \n");
		printf ("Executes 1 Million times ");
		Ticks t1,t2;
		t1 = HighResolutionTimer::GetTicks();
		if (ret){
			for (int j = 0;(j<10) && ret;j++) {
				printf (".");
				fflush(stdout);
				for (int i = 0;(i<100000) && ret;i++) {
					ret = context.Execute(PseudoCode::Context::safeMode,NULL,0);
				}
			}
		}
		t2 = HighResolutionTimer::GetTicks();
		MilliSeconds dT = t2-t1;
		printf (" in %i milliseconds\n",dT.GetTimeRaw());
		fflush(stdout);

	}


	if (!ret){
		printf ("FAILED - see log\n");
	}

 	StartupManager::Terminate();

}

