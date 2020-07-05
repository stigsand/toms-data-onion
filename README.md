# Solution to Tom's Data Onion version 1.0

See https://tomdalling.com/toms-data-onion/

```
mkdir build && cd build && cmake -D CMAKE_BUILD_TYPE=Debug .. && cmake --build . && cat ../start-page.txt | ./layer0 | ./layer1 | ./layer2 | ./layer3
```
