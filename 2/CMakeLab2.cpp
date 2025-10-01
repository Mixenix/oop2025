#include "CMakeLab2.h"
#include "SimpleDomino.h"
#include "ComplexDomino.h"

using namespace std;



void simpleDominoMenu() {
	SimpleDomino dom;
	int choice;
	
	do {
		cout << "\n=== Simple Domino Menu ===" << endl;
		cout << "1. Create domino with specific values" << endl;
		cout << "2. Create random domino" << endl;
		cout << "3. Set random values" << endl;
		cout << "4. Display domino" << endl;
		cout << "5. Display ASCII art" << endl;
		cout << "6. Flip domino" << endl;
		cout << "0. Back to main menu" << endl;
		cout << "Choice: ";
		cin >> choice;
		
		switch(choice) {
			case 1: {
				int val1, val2;
				cout << "Enter two values (0-6): ";
				cin >> val1 >> val2;
				try {
					dom.setVal1(val1);
					dom.setVal2(val2);
					cout << "Domino created: " << dom.getVal1() << "-" << dom.getVal2() << endl;
				} catch (out_of_range& e) {
					cout << "Error: " << e.what() << endl;
				}
				break;
			}
			case 2: {
				dom.setRandomValues();
				cout << "Random domino created: " << dom.getVal1() << "-" << dom.getVal2() << endl;
				break;
			}
			case 3: {
				!dom;
				cout << "Values randomized: " << dom.getVal1() << "-" << dom.getVal2() << endl;
				break;
			}
			case 4: {
				cout << dom << endl;
				break;
			}
			case 5: {
				cout << dom.ascii() << endl;
				break;
			}
			case 6: {
				~dom;
				cout << "Domino flipped: " << dom.getVal1() << "-" << dom.getVal2() << endl;
				break;
			}
			case 0: {
				cout << "Returning to main menu..." << endl;
				break;
			}
			default: {
				cout << "Invalid choice!" << endl;
				break;
			}
		}
	} while (choice != 0);
}

void complexDominoMenu() {
	ComplexDomino comp;
	int choice;
	
	do {
		cout << "\n=== Complex Domino Menu ===" << endl;
		cout << "1. Create empty group" << endl;
		cout << "2. Create group with random dominoes" << endl;
		cout << "3. Create full set" << endl;
		cout << "4. Display group" << endl;
		cout << "5. Add specific domino" << endl;
		cout << "6. Add random domino" << endl;
		cout << "7. Remove random domino" << endl;
		cout << "8. Remove domino by values" << endl;
		cout << "9. Remove domino by index" << endl;
		cout << "10. Sort by sum ascending" << endl;
		cout << "11. Extract subsequence" << endl;
		cout << "12. Display ASCII art for domino" << endl;
		cout << "0. Back to main menu" << endl;
		cout << "Choice: ";
		cin >> choice;
		
		switch(choice) {
			case 1: {
				comp = ComplexDomino(false, 0, -1);
				cout << "Empty group created" << endl;
				break;
			}
			case 2: {
				int size;
				cout << "Enter number of dominoes: ";
				cin >> size;
				try {
					comp = ComplexDomino(true, size, -1);
					cout << "Group with " << size << " random dominoes created" << endl;
				} catch (logic_error& e) {
					cout << "Error: " << e.what() << endl;
				}
				break;
			}
			case 3: {
				int copies;
				cout << "Enter number of copies for each domino: ";
				cin >> copies;
				comp = ComplexDomino(false, 0, copies);
				cout << "Full set created with " << copies << " copies of each domino" << endl;
				break;
			}
			case 4: {
				cout << comp << endl;
				break;
			}
			case 5: {
				int val1, val2;
				cout << "Enter domino values (0-6): ";
				cin >> val1 >> val2;
				try {
					SimpleDomino newDom(val1, val2);
					comp += newDom;
					cout << "Domino added" << endl;
				} catch (out_of_range& e) {
					cout << "Error: " << e.what() << endl;
				}
				break;
			}
			case 6: {
				comp.addRandom();
				cout << "Random domino added" << endl;
				break;
			}
			case 7: {
				comp.removeRandom();
				cout << "Random domino removed" << endl;
				break;
			}
			case 8: {
				int val1, val2;
				cout << "Enter domino values to remove: ";
				cin >> val1 >> val2;
				comp.remove(val1, val2);
				break;
			}
			case 9: {
				int index;
				cout << "Enter index to remove: ";
				cin >> index;
				comp.removeByIndex(index);
				break;
			}
			case 10: {
				comp.sortBySumAscending();
				cout << "Group sorted by sum ascending" << endl;
				break;
			}
			case 11: {
				int value;
				cout << "Enter value to extract: ";
				cin >> value;
				ComplexDomino subseq = comp.subSeq(value);
				cout << "Subsequence extracted:" << endl;
				cout << subseq << endl;
				cout << "Remaining group:" << endl;
				cout << comp << endl;
				break;
			}
			case 12: {
				int index;
				cout << "Enter domino index: ";
				cin >> index;
				try {
					cout << comp.ascii(index) << endl;
				} catch (out_of_range& e) {
					cout << "Error: " << e.what() << endl;
				}
				break;
			}
			case 0: {
				cout << "Returning to main menu..." << endl;
				break;
			}
			default: {
				cout << "Invalid choice!" << endl;
				break;
			}
		}
	} while (choice != 0);
}

int main() {
	int choice;
	
	do {
		cout << "\n=== Domino Test Menu ===" << endl;
		cout << "1. Test SimpleDomino class" << endl;
		cout << "2. Test ComplexDomino class" << endl;
		cout << "3. Run predefined tests" << endl;
		cout << "0. Exit" << endl;
		cout << "Choice: ";
		cin >> choice;
		
		switch(choice) {
			case 1:
				simpleDominoMenu();
				break;
			case 2:
				complexDominoMenu();
				break;
			case 3: {
				cout << "\n=== Running Predefined Tests ===" << endl;
				
				SimpleDomino dom1(3, 4);
				cout << "Domino 1: " << dom1.getVal1() << "-" << dom1.getVal2() << endl;
				
				SimpleDomino dom2(1, 1, true);
				cout << "Domino 2: " << dom2.getVal1() << "-" << dom2.getVal2() << endl;
				
				SimpleDomino dom3(3, 4);
				SimpleDomino dom4(4, 3);
				bool result = (dom3 == dom4);
				cout << "Domino 3 == Domino 4: " << (result ? "true" : "false") << endl;
				
				ComplexDomino comp1(true, 5, -1);
				cout << "Group size: " << comp1.getSize() << endl;
				
				comp1 += dom1;
				cout << "New group size: " << comp1.getSize() << endl;
				
				cout << "All tests completed!" << endl;
				break;
			}
			case 0:
				cout << "Goodbye!" << endl;
				break;
			default:
				cout << "Invalid choice!" << endl;
				break;
		}
	} while (choice != 0);
	
	return 0;
}