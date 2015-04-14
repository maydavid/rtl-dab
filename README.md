# rtl-dab

DAB/DAB+ receiver to use with rtl-sdr sticks

Features:
* FIC decoding
* Simple BER Analysis (more to come)
* Mode I supported

* Ability to inject faults into the receiver. I need this for my PhD thesis to find out if power could be saved by tolerating faults in the receiver. More info: http://davidmay.org/research.html

### This project uses snippets from:

* David Crawley https://github.com/chrisa/opendab GPL, v3
* Phil Karn http://www.ka9q.net/code/fec/ GPL
* Andreas Müller https://github.com/andrmuel/gr-dab GPL, v3
* Makoto Matsumoto http://www.math.sci.hiroshima-u.ac.jp/~m-mat/MT/emt.html


### Status:

#### 01/10/2012

* FIC decoding (i.e. correct crc check) works, however source files need to be cleaned up and added to repository.

#### 05/11/2014

* Finally I found the motivation to work again on this project. The first steps will be to fix the synchronization algorithms

#### 09/11/2014

* Synchronization issues are fixed now. Syncing and tuning is working surprisingly well. ~~There is still an issue with the first data symbol which is used for the FIC. The CRC of the first 4 FIGs is always wrong. Sounds like a simple mapping issue, however I was not able to find the problem.~~



### usage:

 ```
 $ git clone git@github.com:maydavid/rtl-dab.git
 $ cd rtl-dab
 $ cmake .
 $ src/rtl_dab <frequency in Hz>
 ```
 
