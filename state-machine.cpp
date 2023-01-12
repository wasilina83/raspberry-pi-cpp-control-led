namespace local
{
	struct stateEngine_T{
	private:
		// States könnten auch einfach Nummern sein.
		// Auf diese Weise wird der Code aber ausdrücklicher
		using state_T = enum{
			panic_S,
			idle_S,
			state1_S,
			state2_S,
			state3_S,
			exit_S
		};
		state_T nextState_;
		
		// Diese privaten Funktionen würden kapseln, was zu den Transitionen
		// erfüllt sein müsste.
		bool checkForProblems()		{ /*Auswertung*/ return false;};
		bool hasProcessFinished()	{ /*Auswertung*/ return false;};
		bool evaluateTransitionGuard1()	{ /*Auswertung*/ return false;};
		bool evaluateTransitionGuard2()	{ /*Auswertung*/ return false;};
		bool evaluateTransitionGuard3()	{ /*Auswertung*/ return false;};



		// Die State-Funktionen führen aus, was in dem jewiligen State passieren
		// soll. Der return-Wert ist der jeweilige nächste Zustand in den die 
		// Engine versetzt werden soll.
		// Dabei gibt es Zustände welche einfach eine Funktion ausführen und dann
		// die Engine direkt in den nächsten Zustand versetzen.
		state_T panic(){
			//Panikaktion
			return exit_S;
		}
		
		// aber auch Zustände welche in einer Schleife verharren bis ein 
		// Transition-Guard erfüllt ist. 
		// Je nach erfülltem Transition-Guard können die folgenden States 
		// unterschiedlich sein.
		state_T runState1(){
			while(true){
				//Funktionskörper des Zustands
				if(checkForProblems()) return panic_S;
				if(hasProcessFinished()) return idle_S;
			}
		}

		state_T runState2(){
			// Funktionskörper des anderen Zustands
			if(checkForProblems()) return panic_S;
			return idle_S;
		}

		state_T runState3(){
			// Funktionskörper des anderen Zustands
			if(checkForProblems()) return panic_S;
			return idle_S;
		}

		state_T runIdle(){
			while(true){
				//an dieser Stelle könnte auch eine LED blinken
				if(evaluateTransitionGuard1()) return state1_S;
				if(evaluateTransitionGuard2()) return state2_S;
				if(evaluateTransitionGuard3()) return state3_S;
			}
		}

	public:
		int exec(){
			while(nextState_ != exit_S){
				// Die States werden in diesem Switch alle sehr offensichtlich
				// aufgelistet. Um die Transitionsmechanik zu verstehen muss der
				// Programmierer in den jeweiligen Zustand schauen.
				switch(nextState_)
				{
					default:
					case panic_S:
						nextState_ = panic();
						break;
					case idle_S:
						nextState_ = runIdle();
						break;
					case state1_S:
						nextState_ = runState1();
						break;
					case state2_S:
						nextState_ = runState2();
						break;
					case state3_S:
						nextState_ = runState3();
						break;
				}
		

			}
			// Wenn der Exit State erreicht wird, ist etwas schief gelaufen
			// Daher returnt die .exec Funktion eine 1 welche dann von main
			// an den Aufrufenden weitergegeben wird
			return 1;
		}

		stateEngine_T(int argc, char** argv){
			// Konstruktionsroutine die ggf. etwas mit den Übergabeparametern anfängt
			nextState_ = idle_S;
		}
	};
}


int main(int argc, char** argv){
	// Zunächste wird die Statemachine im Speicher angelegt
	local::stateEngine_T states{argc, argv};
	// Im Anschluss daran wird die eigentliche Engine ausgeführt.
	// Der Return-Value der Statemachine ist zugleich der Exit-Status-Code.
	return states.exec();
}
