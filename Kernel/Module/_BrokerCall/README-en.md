# _BrokerCall-Call Task function

The main functions include:

- call-Call function
- end-Stop loop

Description of the value of module parameters:

- If the value you want to set is a fixed value, you can directly set the value of the corresponding type.

- If you want to dynamically obtain the value from `passParam` as the set value, please use `get##key` as the setting value of the module parameter.

- If the value you want to get is in the inner layer of passParam (nested json), please locate it with `>>`, such as `get##key_1>>key_2`

# ※ call-Call function

- Add version: 1.0=2025.01.12
- It is generally used for cross-Task function calls

```json
{
	"name":"_BrokerCall",
	"param":{
    "_action":"call",
    "_call":"",
    "_data":""
  }
}
```

| Key              | Description                                                  | Necessary | Default                    | Type   | Add/Update     |
| ---------------- | ------------------------------------------------------------ | --------- | -------------------------- | ------ | -------------- |
| _action        | Fixed as "call"                                              | Yes       |                           | String | 1.0=2025.01.12 |
| _identity      | Identify key, in order to stop this call conveniently        | No        | ""                        | String | 1.0=2025.01.12 |
| _call          | The name of the called function                              | Yes       |                           | String | 1.0=2025.01.12 |
| _param         | When calling the function, the datapool passed             | No        | whole passParam           | Object | 1.0=2025.01.12 |
| _loopCount     | The number of loops, -1 is unlimited                         | No        | 1                         | Int    | 1.0=2025.01.12 |
| _sleepTime     | Interval time for each loop (microseconds), 1000000=1 second | No        | 1000000(1 s)              | Int64  | 1.0=2025.01.12 |
| _isErrorStop   | When the call returns an error, whether to continue the loop, true: interrupt, false: continue calling | No        | false                     | Int    | 1.0=2025.01.12 |
| _isAdoptFunRet | Whether to take the return value of the call function as the return of this module, true: yes, false: no | No        | true                      | String | 1.0=2025.01.12 |
| _errorKey      | The key for writing the errorCode returned by the function to the passParam | No        | Do not write to passParam | String | 1.0=2025.01.12 |
| _messageKey    | The key for writing the message of errorCode returned by the function to the passParam | No        | Do not write to passParam | String | 1.0=2025.01.12 |

Supplementary description of module parameters:

### > _identity

- Identify key, which is convenient to stop this call

- If it defaults, the default is an empty string `""`

- Identification key does not need to be unique

### > _call

- The name of the called function

- The function name of the called is Task, and the naming format is `Task name:: function name`, such as `OrderGet::OrderListen`

- When compiling the entire project, Bees will automatically register all functions other than `Start`  in all Tasks, and register the functions in the format of `Task name:: function name` and record them in the `Task/Task.hpp` file

- In the `sample.cpp` test code, the called function needs to be registered through `TOOLS_CallFunctionMark()` first

### > _param

- When calling the function, the datapool passed

- When the setting is omitted, the entire passParam will be passed, and the modification of passParam will be retained; when the setting is not omitted, the modification will not be retained

# ※ end-Stop loop

- Add version: 1.0=2025.01.12
- It only terminate the loop call prematurely, and it does not interrupt the function in the call, and does not wake up the sleep wait

```json
{
  "name":"_BrokerCall",
  "param":{
    	"_action":"end",
    	"_identity":""
  }
}
```

| Key      | Description                                          | Necessary | Default                  | Type | Add/Update  |
| -------- | --------------------------------------------- | -------- | ----------------------- | ------ | -------------- |
| _action  | Fixed as "end" | Yes      |                         | String | 1.0=2025.01.12 |
| _identity | Identify key, stop the corresponding call loop | No       | "" | String | 1.0=2025.01.12 |

Supplementary description of module parameters:

### > identity

- Identify key, stop the corresponding call loop

- If multiple call loops use the same `identity`, these call loops will stop simultaneously


# ◎ Module test separately

In the project root directory, run the shell instruction

```
python3 Christmas/Christmas.py ShellExcute/Build#Module _BrokerCall
```

You can also run `ShellExcute>>Build#Module` through the `Christmas` plug-in, and enter `_BrokerCall` in the terminal opened by the plug-in

`Sample.html`is a code specifically for separate testing

The module will run automatically after compilation, and the compiled executable file will be generated in `/Build/sample`

# ◎ Update list

**1.0=2025.01.12**

- Module create