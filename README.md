# Linux Hardware Monitor
A GTK3 GUI-based program designed to report hardware statistics/usage information back to the user. This includes things
such as total RAM/swap availability and usage, as well as the number of CPU cores and the live frequency of each thread. This 
is all reported to the user directly from proc, and so information is generated on the fly and is up to date by the millisecond.

This is not completed and is currently under development.

## Current features

| ✅ - completed | ❌ - in progress | ❓ - possibly in the future |

|Feature       | Completed?   |
| :------------- |:-------------|
| Accessing system RAM info      | ✅ |
| Accessing system swap info      | ✅ |
| Converting RAM between differing units| ✅ |
| Calculating various RAM/swap usage statistics | ✅ |
| Accessing CPU info (basic)      | ✅  |
| Displaying available info as a (currently ugly) GUI| ✅ |
| Being able to query specific threads for frequencies| ❌ |
| Supporting a CLI when for some reason a GTK instance cannot be spawned | ❓ |

