# Solution to Tom's Data Onion version 1.0 and 1.1

See https://tomdalling.com/toms-data-onion/

Build requirements:
sudo apt-get install libssl-dev

```
mkdir build && cd build && cmake -D CMAKE_BUILD_TYPE=Debug .. && cmake --build . && cat ../start-page.txt | ./layer0 | ./layer1 | ./layer2 | ./layer3 | ./layer4 | ./layer5
```
