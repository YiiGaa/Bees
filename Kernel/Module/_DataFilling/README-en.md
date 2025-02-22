# _DataFilling-Data pool passParam handling

The main functions include:

- set-Insert data in passParam
- save-Save data

Description of the value of module parameters:

- If the value you want to set is a fixed value, you can directly set the value of the corresponding type.

- If you want to dynamically obtain the value from `passParam` as the set value, please use `get##key` as the setting value of the module parameter.

- If the value you want to get is in the inner layer of passParam (nested json), please locate it with `>>`, such as `get##key_1>>key_2`

# ※ set-Insert data in passParam

- Add version: 1.0=2025.02.09
- Multiple keys can be set at the same time

```json
{
    "name":"_DataFilling",
    "param":{
    	"_action":"set",
        "_setting":{
            "key-1":"uuid",
			"opt##key-2":"time",
          	"key-1>>key-1-1":"get##key",
          	"switch##key-2":{
              	"reg##xxx":"new value",
              	"xxx":"value"
              	"":"else value"
            }
        }
    }
}
```

| Key    | Description                                                         | Necessary | Default | Type | Add/Update  |
| ------ | ------------------------------------------------------------ | -------- | ------ | ------ | -------------- |
| _action | Fixed to "set"                                | Yes       |        | String | 1.0=2025.02.09 |
| _setting | Template for modifying data | Yes       |        | Object | 1.0=2025.02.09 |
| _isSwitchNullError | When the `switch##` conversion value is not successful, whether to report an error, false: no error, keep the original value, true: error exit | No | true | Bool | 1.0=2025.02.09 |

Supplementary description of module parameters:

### > _setting

- Template for modifying data, type Json Object

- The module does not directly overwrite the target data pool, but traverses the Json Object and inserts the data into the target data pool one by one (passParam)

- If you want to clean/limit the data of the data pool (passParam), please use the `_DataCheck` module

- Templates can set up multi-layer nested relationships, but in order to make it easier to understand, please try to limit it to a single layer

- The following is a detailed description of the Key/Value of the template

### > Key description of the template

- The Key corresponds to the key of the `passParam`

- The Key can add `nec##` or `opt##` as the prefix to indicate whether to force the modification of parameters, `nec#` is the forced modification (default), and `opt##` is non-compulsory (not modified when it exists)

- Key can add `push##` as the prefix, and when the Value and target data of the template are array type `[]`, it indicates the array addition mode, and new data will be pushed to the target array

- Key can add `switch##` as the prefix, and when the Value of the template is the object type `{}`, it indicates the value conversion mode. When the conversion fails, if `_isSwitchNullError` is set to `false`, the original value is retained. If `_isSwitchNullError` is set to `true`, the error will be reported to exit

    ```
    #The value of passParam
    {
    		"key 1":true,
    		"key 2":"good",
    		"key 3":"video"
    }
    
    #Adopt the conversion mode to convert, add the prefix `reg##` to indicate the regular expression, and `""` indicates the default value
    {
    		"switch##key 1":{
    				"true":"It is right",
    				"false":"It is wrong"
    		},
    		"switch##key 2":{
    				"reg##go+d":"good job"
    		},
    		"switch##key 3":{
    				"audio":"audio type",
    				"":"video type"
    		}
    }
    
    #The final passParam value
    {
    		"key 1":"It is right",
    		"key 2":"good job",
    		"key 3":"video type"
    }
    ```

- The prefix `switch##` and `push##` cannot exist at the same time

- If there are multiple prefixes, please pay attention to their order. `nec##` needs to be before `switch##`, for example: `nec##switch##key`

- The key can be dynamically defined by using the syntax of `Value of the template`, and can be wrapped with `@@`. For example, `key_@get##id@`, the module will automatically replace the data of `@get##id@` part. The internal syntax description of `@@` refers to the special description of `Value description of the template`

- Key can use `>>` to locate the nested relationship, such as `xxx>>yyy>>1>>zz`, indicating that it is located to the `zz` field

    ```
    {
    	"xxx":{
    		"yyy":[
    			{},
    			{"zz":""}
    		]
    	}
    }
    ```

- When using `>>` positioning, the default `number` represents the index of the array, and the string is the key of the object. If the multi-layer location is not found, the object will be automatically created.

- The value corresponding to key is the array `[]`, and the target data is an empty array `[]` (or the field does not exist), then it will be inserted data.

    ```
    #setting
    {
    	"xxx":[
    		"key",
    		{"key":"value"}
    	]
    }
    
    #The target data is an empty array, or the field xxx does not exist
    {
    		"xxx":[]
    }
    
    #Data results
    {
    	"xxx":[
    		"key",
    		{"key":"value"}
    	]
    }
    ```

- When the value corresponding to Key is the array `[]`, and the target data is a non-empty array `[]`, the target array will be overwritten

- When the value corresponding to the key is the array `[]`, and the target data is a non-empty array `[]`, the prefix `push##` can be added, which is the inserted data. When the key has multiple prefixes, the correct prefix order is `opt##push##key`

    ```
    #setting
    {
    	"push##xxx":[
    		"key",
    		{"key":"value"}
    	]
    }
    
    #Target data, non-empty array
    {
    		"xxx":[
    			"yyy"
    		]
    }
    
    #Data results
    {
    	"xxx":[
    		"yyy",
    		"key",
    		{"key":"value"}
    	]
    }
    ```

### > Value description of the template

- Value is the modified value, and Value can be int, double, bool, string, {}, [] and other explicit values

- Value is set to `null`, which means that this Key will be deleted

- When the Value is set to `{}`、`[]`, the inner key and value can be set internally to set the deep data modification

- When the Value is set to String, the processing data expression can be set to achieve complex data settings

- Examples of expressions for processing data: get##key>>key_1+uuid

- The data processing fragment is divided by `+`, and each data processing fragment is processed separately. After processing, multiple fragments are stitched into String

    > When you need to output `+`, use the `++` , when you need to output `++`, use the `+++` , and so on. Add version: 1.0=2025.02.09

- Each data processing fragment is divided by `##`. The previous keyword is the function, and the following keyword is the parameter setting of the corresponding function. Some functions do not require parameter setting, and `##` can be ignored

- If the following functions cannot be matched, it will be directly used as a spliced string

    | Function  | Description                                                  | Setting                                                      | Add/Update     |
    | --------- | ------------------------------------------------------------ | ------------------------------------------------------------ | -------------- |
    | uuid      | 32-bit uuid                                                  | -                                                            | 1.0=2025.02.09 |
    | random id | Random number (default 8 bits)                               | Random number length (default 8 bits)                        | 1.0=2025.02.09 |
    | data      | Get data from storage (_action:save)                         | The key of storing data. And it can use `>>` to locate multi-layer search. If it does not exist, the "null" string is used. | 1.0=2025.02.09 |
    | get       | Get the value from passParam                                 | The key in passParam. And it can use `>>` to locate multi-layer search. If it does not exist, use the "null" string | 1.0=2025.02.09 |
    | time      | Time, the default format is %Y-%m-%d %H:%M:%S                | Time format                                                  | 1.0=2025.02.09 |
    | timeStamp | Get the current timestamp (the number of seconds started from 1970) | -                                                            | 1.0=2025.02.09 |
    | getPid    | Get the current thread ID                                    | -                                                            | 1.0=2025.02.09 |

# ※ save-Save data

- Add version: 1.0=2025.02.09
- The data is stored in the module, and the global praise thread is effective

```
{
    "name":"_DataFilling",
    "param":{
    	"_action":"save",
        "_setting":{
        }
    }
}
```
| Key      | Description                | Necessary | Default | Type   | Add/Update     |
| -------- | -------------------------- | --------- | ------- | ------ | -------------- |
| _action  | Fixed as "save"            | Yes       |         | String | 1.0=2025.02.09 |
| _setting | Templates for storing data | Yes       |         | Object | 1.0=2025.02.09 |
Supplementary description of module parameters:

### > _setting

- Templates for storing data, type Json Object
- For detailed instructions, please refer to "set-Insert data in passParam"

# ◎ Module test separately

In the project root directory, run the shell instruction

```
python3 Christmas/Christmas.py ShellExcute/Build#Module _DataFilling
```

You can also run `ShellExcute>>Build#Module` through the `Christmas` plug-in, and enter `_DataFilling` in the terminal opened by the plug-in

`Sample.html`is a code specifically for separate testing

The module will run automatically after compilation, and the compiled executable file will be generated in `/Build/sample`

# ◎ Update list

1.0=2025.02.09

- Module create