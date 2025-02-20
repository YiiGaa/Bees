# _BrokerThread-Threading operation(run Task)

The main functions include:

- create-Create thread
- wait-Wait for a thread to end

- wait all-Wait for all threads to end

Description of the value of module parameters:

- If the value you want to set is a fixed value, you can directly set the value of the corresponding type.

- If you want to dynamically obtain the value from `passParam` as the set value, please use `get##key` as the setting value of the module parameter.

- If the value you want to get is in the inner layer of passParam (nested json), please locate it with `>>`, such as `get##key_1>>key_2`

# ※ create-Create thread

- Add version: 1.0=2025.01.12
- Creating a thread is actually starting a thread and calling the `Start()` function of `Task`

```
{
	"name":"_BrokerThread",
	"param":{
		"_action":"create",
		"_identity":"",
		"_call":""
	}
}
```

| Key        | Description                                                         | Necessary | Default                                                   | Type | Add/Update  |
| ---------- | ------------------------------------------------------------ | -------- | -------------------------------------------------------- | ------ | -------------- |
| _action | Fixed as "create"                                | Yes       |                                                          | String | 1.0=2025.01.12 |
| _identity | Identify key, require unique. Generally, the same value as `_call` is used | Yes       |                                                          | String | 1.0=2025.01.12 |
| _call | The `Task` name to be called | No       | Pass the empty json object | String | 1.0=2025.01.12 |
| _param | When calling the function, the datapool passed | No | whole passParam | Object | 1.0=2025.01.12 |

Supplementary description of module parameters:

### > _call

- The `Task` name to be called

- Creating a thread is actually to start a thread and call the `Start` function of the `Task`

- When compiling the entire project, all `Task` containing the `Start` function will be automatically registered and recorded in the `Task/Task.hpp` file. If the `Start` function in `Task` is removed, it will not be automatically registered

- In the `sample.cpp` test code, you need to register through `TOOLS_CallThreadMark()`

### > _param

- When calling the function, the datapool passed

- Before starting the thread, a new copy will be cloned, which will not affect the data pool of this thread

# ※ wait-Wait for a thread to end

- Add version: 1.0=2025.01.12
- Hang up the current thread until a thread ends

```
{
	"name":"_BrokerThread",
	"param":{
		"_action":"wait",
		"_identity":""
	}
}
```

| Key        | Description                                                         | Necessary | Default                                                   | Type | Add/Update  |
| ---------- | ------------------------------------------------------------ | -------- | -------------------------------------------------------- | ------ | -------------- |
| _action | Fixed as "wait"          | Yes       |                                                          | String | 1.0=2025.01.12 |
| _identity | Identify key, corresponding to the id at the time of creation | Yes    |  | String | 1.0=2025.01.12 |

# ※ wait all-Wait for all threads to end

- Add version: 1.0=2025.01.12
- Hang the current thread until all threads are over

```
{
	"name":"_BrokerThread",
	"param":{
		"_action":"wait all"
	}
}
```

| Key    | Description                        | Necessary | Default | Type | Add/Update  |
| ------ | --------------------------- | -------- | ------ | ------ | -------------- |
| _action | Fixed as "wait all" | Yes       |        | String | 1.0=2025.01.12 |

# ◎ Module test separately

In the project root directory, run the shell instruction

```
python3 Christmas/Christmas.py ShellExcute/Build#Module _BrokerThread
```

You can also run `ShellExcute>>Build#Module` through the `Christmas` plug-in, and enter `_BrokerThread` in the terminal opened by the plug-in

`Sample.html`is a code specifically for separate testing

The module will run automatically after compilation, and the compiled executable file will be generated in `/Build/sample`

# ◎ Update list

**1.0=2025.01.12**

- Module create