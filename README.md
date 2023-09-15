# <img src="https://diabetes.zcu.cz/img/icon.png" width="24" height="24" /> SmartCGMS - common
This repository contains shared base implementations. Every SmartCGMS module directly depends on this repository.

SmartCGMS software architecture and framework.
Project homepage: [diabetes.zcu.cz](https://diabetes.zcu.cz/smartcgms)
## Repository structure
- `cmake` - functions and macros required by the shared buildsystem
- `scgms`
	- `iface` - interface definitions, constants and behavior definitions
	- `lang` - system-wide strings and identifiers
	- `resource` - common resource files
	- `rtl` - base implementations of entities, bridges and similar; often refered to as to a Software Development Kit (SDK)
	- `utils` - utility implementations (e.g., string conversions, filesystem bridges), OS abstraction layer and similar

## License

The SmartCGMS software and its components are distributed under the Apache license, version 2. When publishing any derivative work or results obtained using this software, you agree to cite the following paper:

_Tomas Koutny and Martin Ubl_, "SmartCGMS as a Testbed for a Blood-Glucose Level Prediction and/or Control Challenge with (an FDA-Accepted) Diabetic Patient Simulation", Procedia Computer Science, Volume 177, pp. 354-362, 2020

See attached LICENSE file for full licencing information.

|![University of West Bohemia](https://www.zcu.cz/en/assets/logo.svg)|![Department of Computer Science and Engineering](https://www.kiv.zcu.cz/site/documents/verejne/katedra/dokumenty/dcse-logo-barevne.png)|
|--|--|