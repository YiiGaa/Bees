# _DataQueue-Data queue

The main functions include:

- create-Create queue
- push-Store data

- pull-Get data

- pop-Destruct data

- delete-Delete queue

Description of the value of module parameters:

- If the value you want to set is a fixed value, you can directly set the value of the corresponding type.

- If you want to dynamically obtain the value from `passParam` as the set value, please use `get##key` as the setting value of the module parameter.

- If the value you want to get is in the inner layer of passParam (nested json), please locate it with `>>`, such as `get##key_1>>key_2`

# ※ create-Create queue

- Add version: 1.0=2025.01.12

```
{
	"name":"_DataQueue",
	"param":{
		"_action":"create",
		"_identity":"",
		"_queueMaxLen":10
	}
}
```

| Key       | Description                       | Necessary | Default     | Type | Add/Update  |
| --------- | -------------------------- | -------- | ---------- | ------ | -------------- |
| _action | Fixed to "create" | Yes       |            | String | 1.0=2025.01.12 |
| _identity | Queue Identity, require uniqueness | Yes       |            | String | 1.0=2025.01.12 |
| _queueMaxLen | Maximum length of queue | No       | -1(No limit) | Int    | 1.0=2025.01.12 |

# ※ push-Store data

- Add version: 1.0=2025.01.12

```
{
	"name":"_DataQueue",
	"param":{
		"_action":"push",
		"_identity":"",
		"_data":""
	}
}
```

| Key      | Description                                                         | Necessary | Default                    | Type | Add/Update  |
| -------- | ------------------------------------------------------------ | -------- | ------------------------- | ------ | -------------- |
| _action | Fixed as "push"                                  | Yes       |                           | String | 1.0=2025.01.12 |
| _identity | Queue Identity                                       | Yes       |                           | String | 1.0=2025.01.12 |
| _data  | Stored data | Yes    |  | Open type | 1.0=2025.01.12 |
| _isFullWait | Whether to wait when the queue is full, true:wait, false:no wait | No       | true                    | Bool | 1.0=2025.01.12 |
| _timeout | Wait for timeout time (seconds), if a timeout occurs, the function `_timeoutCall` will be called, and then continue to wait, repeating this cycle | No       | Keep waiting | Int    | 1.0=2025.01.12 |
| _timeoutCall | Function for waiting timeout call, passing current passParam | No       |                           | String | 1.0=2025.01.12 |

Supplementary description of module parameters:

### > _isFullWait

- Whether to wait when the queue is full, true:wait, false:no wait

- Set to wait `true`, and the data will be put into the queue first, and then wait until a data is destroyed

- Set not to wait for `false`, and the data will be put into the queue first, and then forcibly clean up the first data of the queue

### > _timeoutCall

- Wait for the function to be called over timed and pass the current passParam

- The function named `Task` is called, and the naming format is `Task Name::Function Name`, such as `OrderGet::OrderListen`

- When compiling the entire project, all functions other than `Start` in Task will be automatically registered, and the functions will be registered in the format of `Task name::function name` and recorded in the `Task/Task.hpp` file

- In the `sample.cpp` test code, the called function needs to be registered through `TOOLS_CallFunctionMark()` first

# ※ pull-Get data

- Add version: 1.0=2025.01.12
- The data entered first will be retrieved first

```
{
	"name":"_DataQueue",
	"param":{
		"_action":"pull",
		"_identity":"",
		"_resultKey":""
	}
}
```

| Key          | Description                                                  | Necessary | Default      | Type   | Add/Update     |
| ------------ | ------------------------------------------------------------ | --------- | ------------ | ------ | -------------- |
| _action      | Fixed to "pop"                                               | Yes       |              | String | 1.0=2025.01.12 |
| _identity    | Queue Identity                                               | Yes       |              | String | 1.0=2025.01.12 |
| _resultKey   | The retrieved data is stored in the key of passParam         | Yes       |              | String | 1.0=2025.01.12 |
| _dataIdKey   | Store the data Id to the key of passParam                    | No        | dataId       | String | 1.0=2025.01.12 |
| _isPop       | Whether to destroy the data immediately, true: yes, false: no | No        | true         | String | 1.0=2025.01.12 |
| _isEmptyWait | When the queue is empty, wait or not, true: wait, false: do not wait | No        | true         | Bool   | 1.0=2025.01.12 |
| _timeout     | Wait for timeout time (seconds), if a timeout occurs, the function `_timeoutCall` will be called, and then continue to wait, repeating this cycle | No        | Keep waiting | Int    | 1.0=2025.01.12 |
| _timeoutCall | Function for waiting timeout call, passing current passParam | No        |              | String | 1.0=2025.01.12 |

Supplementary description of module parameters:

### > _isPop

- Whether to destroy the data immediately, true: yes, false: no

- Set to `true`, and the data in the queue will be removed immediately

- Set to `false`, data will not be removed from the queue, only marked `in use`

- The data of `in use` will not be obtained repeatedly, but will occupy the queue position

### > _dataIdKey

- Store the data Id in the key of passParam

- Data Id is a growing `Int` data to facilitate the removal of the corresponding `in use` data when calling the `pop-destruct data` function of this module

- When `_isPop` is set to `true`, this setting is invalid

### > _isFullWait

- When the queue is empty, whether to wait, true: wait, false: don't wait

- Set to wait `true`. When the queue is empty, enter the wait until a certain data is stored.

- Set not to wait for `false`. When the queue is empty, an error will be reported.

### > _timeoutCall

- Wait for the function to be called over timed and pass the current passParam

- The function named `Task` is called, and the naming format is `Task Name::Function Name`, such as `OrderGet::OrderListen`

- When compiling the entire project, all functions other than `Start` in Task will be automatically registered, and the functions will be registered in the format of `Task name::function name` and recorded in the `Task/Task.hpp` file

- In the `sample.cpp` test code, the called function needs to be registered through `TOOLS_CallFunctionMark()` first

# ※ pop-Destruct data

- Add version: 1.0=2025.01.12
- Only used to destroy the data in the `in use` state

```
{
	"name":"_DataQueue",
	"param":{
    	"_action":"pop",
    	"_identity":""
	}
}
```

| Key       | Description                                                         | Necessary | Default                          | Type | Add/Update  |
| --------- | ------------------------------------------------------------ | -------- | ------------------------------- | ------ | -------------- |
| _action | Fixed as "pop"                                   | Yes       |                                 | String | 1.0=2025.01.12 |
| _identity | Queue Identity                                       | Yes       |                                 | String | 1.0=2025.01.12 |
| _id | The id of data will be destroyed | No | Destroy the fist data in the  `in use` state | String | 1.0=2025.01.12 |
| _isNotFoundError | Failed to destroy, whether to report an error, true: report an error, false: do not report an error | No | false | Bool | 1.0=2025.01.12 |

Supplementary description of module parameters:

### > _id

- The id of the destroyed data

- If it is not set, the fist data in the  `in use` state will be destroyed

# ※ delete-Delete queue

- Add version: 1.0=2025.01.12
- Automatically wake up all waiting threads, and these waiting threads will report errors

```
{
	"name":"_DataQueue",
	"param":{
    	"_action":"delete",
    	"_identity":""
	}
}
```

| Key       | Description       | Necessary | Default | Type   | Add/Update     |
| --------- | ----------------- | --------- | ------- | ------ | -------------- |
| _action   | Fixed as "delete" | Yes       |         | String | 1.0=2025.01.12 |
| _identity | Queue Identity    | Yes       |         | String | 1.0=2025.01.12 |

Supplementary description of module parameters:

### > _identity

- Queue identification

- The deletion queue is not completed immediately, unless there are no waiting threads at present

- If there are waiting threads, the last awakened thread will complete the queue deletion

# ◎ Module test separately

In the project root directory, run the shell instruction

```
python3 Christmas/Christmas.py ShellExcute/Build#Module _DataQueue
```

You can also run `ShellExcute>>Build#Module` through the `Christmas` plug-in, and enter `_DataQueue` in the terminal opened by the plug-in

`Sample.html`is a code specifically for separate testing

The module will run automatically after compilation, and the compiled executable file will be generated in `/Build/sample`

# ◎ Update list

**1.0=2025.01.12**

- Module create