# _BrokerLogger-Output custom logs

The main functions include:

- info-Output info log
- error-Output error log

- debug-Output debug log

- console-Do not use formatting, output info log directly

Description of the value of module parameters:

- If the value you want to set is a fixed value, you can directly set the value of the corresponding type.

- If you want to dynamically obtain the value from `passParam` as the set value, please use `get##key` as the setting value of the module parameter.

- If the value you want to get is in the inner layer of passParam (nested json), please locate it with `>>`, such as `get##key_1>>key_2`

# ※ info-Output info log

- Add version: 1.0=2025.01.12

```
{
	"name":"_BrokerLogger",
	"param":{
    "_action":"info",
		"_message":""
	}
}
```

| Key          | Description                                      | Necessary | Default | Type   | Add/Update     |
| ------------ | ------------------------------------------------ | --------- | ------- | ------ | -------------- |
| action       | Fixed as "info"   | Yes        |         | String | 1.0=2025.01.12 |
| message      | Printed log message             | Yes        |         | String | 1.0=2025.01.12 |
| isPrintParam | Whether to print moduleParam and passParam, false: do not print, true: print | No        | false  | Bool | 1.0=2025.01.12 |

Supplementary description of module parameters:

### > _message

- The final output format is `[time][info][thread number][task location][location] log string, called code location,moduleParam:{},passParam:{}`


# ※ error-Output error log

- Add version: 1.0=2025.01.12

```
{
	"name":"_BrokerLogger",
	"param":{
    "_action":"error",
		"_message_":""
	}
}
```

| Key          | Description                                      | Necessary | Default | Type | Add/Update     |
| ------------ | ------------------------------------------------ | -------- | ------ | ------ | -------------- |
| action       | Fixed as "error" | Yes       |        | String | 1.0=2025.01.12 |
| message      | Printed log message                   | Yes       |        | String | 1.0=2025.01.12 |
| isPrintParam | Whether to print moduleParam and passParam, false: do not print, true: print | No       | false | Bool | 1.0=2025.01.12 |

Supplementary description of module parameters:

### > _message

- The final output format is `[time][error][thread number][task location][location] log string, called code location,moduleParam:{},passParam:{}`

# ※ debug-Output debug log

- Add version: 1.0=2025.01.12
- Debug log will not output the log when it is officially running

```
{
	"name":"_BrokerLogger",
	"param":{
    "_action":"debug",
		"_message_":""
	}
}
```

| Key          | Description                                                  | Necessary | Default | Type   | Add/Update     |
| ------------ | ------------------------------------------------------------ | --------- | ------- | ------ | -------------- |
| action       | Fixed as "console"                                           | Yes       |         | String | 1.0=2025.01.12 |
| message      | Printed log message                                          | Yes       |         | String | 1.0=2025.01.12 |
| isPrintParam | Whether to print moduleParam and passParam, false: do not print, true: print | No        | false   | Bool   | 1.0=2025.01.12 |

Supplementary description of module parameters:

### > _message

- The final output format is `[time][debug][thread number][task location][location] log string, called code location,moduleParam:{},passParam:{}`

# ※ console-Output console log

- Add version: 1.0=2025.01.12

```
{
	"name":"_BrokerLogger",
	"param":{
    "_action":"console",
		"_message_":""
	}
}
```

| Key     | Description         | Necessary | Default | Type   | Add/Update     |
| ------- | ------------------- | --------- | ------- | ------ | -------------- |
| action  | Fixed as "console"  | Yes       |         | String | 1.0=2025.01.12 |
| message | Printed log message | Yes       |         | String | 1.0=2025.01.12 |

Supplementary description of module parameters:

### > _message

- This log does not have the format of the above log

# ◎ Module test separately

In the project root directory, run the shell instruction

```
python3 Christmas/Christmas.py ShellExcute/Build#Module _BrokerLogger
```

You can also run `ShellExcute>>Build#Module` through the `Christmas` plug-in, and enter `_BrokerLogger` in the terminal opened by the plug-in

`Sample.html`is a code specifically for separate testing

The module will run automatically after compilation, and the compiled executable file will be generated in `/Build/sample`

# ◎ Update list

**1.0=2025.01.12**

- Module create