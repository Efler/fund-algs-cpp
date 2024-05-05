----------------------------------------------------------------------------------------------
                               --- Database (coursework) ---
----------------------------------------------------------------------------------------------


1) Database has two methods:

    run_file_commands(file_path) - reads commands from a file and executes them sequentially.
    (?) Note: If a format/internal error occurs during the execution of commands, the method
        throws a logic_error exception and closes the file.
        Subsequent commands from the error location will not be executed.
    (?) Note: Empty lines or partial commands entry in a file are not allowed.

    start_dialog() - opens an interactive dialog and controls all execution errors.


2) Commands and their input format:
(?) Note: The command format description contains notes (optional) that are not related to
    the command format and are written for clarification only. When entering a command, ignore
    the (optional) marks.

    1. Insert:
        Format {
            insert: [pool_name] [scheme_name] [collection_name]
            key: [id] [build_version]
            value: [hash] [developer_login] [developer_email] [assembly_script_path]
            [assembly_name] [build_errors] [static_code_analysis_errors] [autotest_run_errors]
            [assembly_artifacts_dir]

            (?) Note: the value string is divided into three parts for easy reading.
                While entering a string of values write only one line!
            (?) Note: All passed components are strings (except for id, must be a number!).
        }
        Description {
            Adds an entry to the specified collection.
        }

    2. Read key:
        Format {
            read key: [pool_name] [scheme_name] [collection_name]
            key: [id] [build_version]

            (?) Note: All passed components are strings (except for id, must be a number!).
        }
        Description {
            Outputs the value of a key entry in the specified collection to the console.
        }

    3. Read range:
        Format {
            read range: [pool_name] [scheme_name] [collection_name]
            keys: [id, build_version](optional) [id, build_version](optional)

            (?) Note: All passed components are strings (except for id, must be a number!).
            (?) Note: In the keys string passes the minimum key, and then the maximum key to read.
                It is allowed not to pass any of the bounds, in this case the reading will be done
                from the beginning/end of the collection. If you don't want to pass the key, just
                leave the empty brackets [].
        }
        Description {
            Outputs the values in the range between the minimum and maximum bounds in the specified
            collection to the console.
        }

    4. Update key:
        Format {
            update key: [pool_name] [scheme_name] [collection_name]
            key: [id] [build_version]
            value: [hash] [developer_login] [developer_email] [assembly_script_path]
            [assembly_name] [build_errors] [static_code_analysis_errors] [autotest_run_errors]
            [assembly_artifacts_dir]

            (?) Note: the value string is divided into three parts for easy reading.
                While entering a string of values write only one line!
            (?) Note: All passed components are strings (except for id, must be a number!).
        }
        Description {
            Updates an entry of the specified collection using a new value.
        }

    5. Remove:
        Format {
            remove: [pool_name] [scheme_name] [collection_name]
            key: [id] [build_version]

            (?) Note: All passed components are strings (except for id, must be a number!).
        }
        Description {
            Removes an entry in the specified collection.
        }

    6. Add pool:
        Format {
            add pool: [pool_name] { [alloc_name] [size(only if needed)] [alloc_mode(only if exists)] }

            (?) Note: Pool_name - a string. Alloc_name - one of this names: global heap, sorted list,
                border descriptor, buddies. Size - a positive number. If you choose a global heap, do
                not write size with [] brackets. If you choose a sorted list or a border descriptor
                allocators, also select a mode - first, best or worst. If you choose another allocator,
                do not write [] brackets and its content.
        }
        Description {
            Adds a new pool to the database, for which the selected allocator will be used.
        }

    7. Delete pool:
        Format {
            delete pool: [pool_name]

            (?) Note: Pool_name - a string.
        }
        Description {
            Deletes the pool in the database.
        }

    8. Add scheme:
        Format {
            add scheme: [pool_name] [scheme_name]

            (?) Note: All passed components are strings.
        }
        Description {
            Adds a new scheme to the existing pool.
        }

    9. Delete scheme:
        Format {
            delete scheme: [pool_name] [scheme_name]

            (?) Note: All passed components are strings.
        }
        Description {
            Deletes the scheme in the existing pool.
        }

    10. Add collection:
        Format {
            add collection: [pool_name] [scheme_name] [collection_name] {tree_name}

            (?) Note: All passed components are strings. Tree name - one of the name of the
                container: avl tree or red black tree.
        }
        Description {
            Adds a new collection to the existing scheme and sets the associative container
            for this collection.
        }

    11. Delete collection:
        Format {
            delete collection: [pool_name] [scheme_name] [collection_name]

            (?) Note: All passed components are strings.
        }
        Description {
            Deletes the collection in the existing scheme.
        }

    12. Reset database:
        Format {
            reset database:
        }
        Description {
            Resets the database by deleting all collections, schemes and pools.
        }

----------------------------------------------------------------------------------------------
                                    --- Thank You! ---
----------------------------------------------------------------------------------------------
