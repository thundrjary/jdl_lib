/* jd_satools.h */

// ==========================================================
// JD_SATOOLS Introduction
// ==========================================================

/**
 * @file jd_satools.h
 * @brief Single file header, abstracting and wrapping GLFW API for simplicity.
 *
 * This header file provides an abstraction layer over GLFW to simplify its usage
 * in graphics programming. It aims to reduce the complexity of using GLFW directly
 * and to make the code more readable and maintainable.
 *
 * Usage:
 * Define JD_GFX_IMPLEMENTATION in one .c/.cpp file before including this header file
 * to create the implementation in that file.
 */

# define JD_SATOOLS_IMPLEMENTATION

#ifndef JD_SATOOLS_H
#define JD_SATOOLS_H

#ifndef JD_SATOOLS_IMPLEMENTATION
    #define JD static
#else
    #define JD extern
#endif

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "jd_debug.h"


#ifndef SA_TOOLKIT_H
#define SA_TOOLKIT_H

#ifdef SA_TOOLKIT_IMPLEMENTATION
   #define SA_API static
#else
   #define SA_API extern
#endif

#include <stdint.h>
#include <stdbool.h>

/*
 * # Ideas:
 *
 * File Management (`jd_satools_file`)**
 * - `jd_satools_file.copy(src, dest, overwrite)`
 * - `jd_satools_file.delete(path)`
 * - `jd_satools_file.get_attributes(path)`
 * - `jd_satools_file.set_attributes(path, attr_flags)`
 * - `jd_satools_file.read(path, buffer, size)`
 * - `jd_satools_file.write(path, data, size, mode)`
 * - `jd_satools_file.exists(path)`
*
 * Directory Management (`jd_satools_dir`)**
 * - `jd_satools_dir.create(path)`
 * - `jd_satools_dir.delete(path, recursive)`
 * - `jd_satools_dir.list(path, filter)`
 * - `jd_satools_dir.size(path)`
 *
 * Process Management (`jd_satools_proc`)**
 * - `jd_satools_proc.start(command, args, pid_out)`
 * - `jd_satools_proc.kill(pid, signal)`
 * - `jd_satools_proc.status(pid)`
 * - `jd_satools_proc.list(filter)`
 * - `jd_satools_proc.priority(pid, level)`
 * - `jd_satools_proc.uptime(pid)`
*
 * Networking (`jd_satools_net`)**
 * - `jd_satools_net.hostname()`
 * - `jd_satools_net.resolve(hostname)`
 * - `jd_satools_net.interfaces()`
 * - `jd_satools_net.ping(address, timeout)`
 * - `jd_satools_net.port_check(host, port)`
 * - `jd_satools_net.http_get(url, headers, response)`
*
 * User Management (`jd_satools_user`)**
 * - `jd_satools_user.get_name(uid)`
 * - `jd_satools_user.get_home(uid)`
 * - `jd_satools_user.list()`
 * - `jd_satools_user.add(username, options)`
 * - `jd_satools_user.remove(username)`
*
 * Permissions (`jd_satools_perm`)**
 * - `jd_satools_perm.get(path)`
 * - `jd_satools_perm.set(path, permissions)`
 * - `jd_satools_perm.chown(path, user, group)`
 * - `jd_satools_perm.chmod(path, mode)`
*
 * System Information (`jd_satools_sys`)**
 * - `jd_satools_sys.os_name()`
 * - `jd_satools_sys.cpu_count()`
 * - `jd_satools_sys.memory_info()`
 * - `jd_satools_sys.disk_usage(path)`
 * - `jd_satools_sys.uptime()`
*
 * Configuration Management (`jd_satools_config`)**
 * - `jd_satools_config.read(file, key, value_out)`
 * - `jd_satools_config.write(file, key, value)`
 * - `jd_satools_config.list_keys(file)`
 * - `jd_satools_config.remove_key(file, key)`
*
 * Logging (`jd_satools_log`)**
 * - `jd_satools_log.write(level, message)`
 * - `jd_satools_log.read(file, filter)`
 * - `jd_satools_log.clear(file)`
*
 * Encryption and Security (`jd_satools_crypto`)**
 * - `jd_satools_crypto.hash(data, algorithm)`
 * - `jd_satools_crypto.encrypt(data, key, options)`
 * - `jd_satools_crypto.decrypt(data, key, options)`
 * - `jd_satools_crypto.generate_key(length)`
 *
 * Task Scheduling (`jd_satools_sched`)**
 * - `jd_satools_sched.add(name, command, interval)`
 * - `jd_satools_sched.remove(name)`
 * - `jd_satools_sched.list()`
 * - `jd_satools_sched.run_now(name)`
*
 * Backup and Restore (`jd_satools_backup`)**
 * - `jd_satools_backup.create(src, dest, options)`
 * - `jd_satools_backup.restore(backup_file, dest)`
 * - `jd_satools_backup.validate(backup_file)`
*
 * Monitoring (`jd_satools_monitor`)**
 * - `jd_satools_monitor.cpu_usage(interval)`
 * - `jd_satools_monitor.memory_usage(interval)`
 * - `jd_satools_monitor.disk_io(interval)`
 * - `jd_satools_monitor.network_io(interval)`
 * - `jd_satools_monitor.process(pid)`
*
 * Automation (`jd_satools_automation`)**
 * - `jd_satools_automation.script_run(script_path)`
 * - `jd_satools_automation.task_chain(tasks)`
 * - `jd_satools_automation.rollback_on_error()`
*
 * Time and Date (`jd_satools_time`)**
 * - `jd_satools_time.now()`
 * - `jd_satools_time.format(timestamp, format)`
 * - `jd_satools_time.sleep(seconds)`
 * - `jd_satools_time.parse(date_string, format)`
 *
 * System Control (`jd_satools_control`)**
 * - `jd_satools_control.shutdown()`
 * - `jd_satools_control.reboot()`
 * - `jd_satools_control.service_start(name)`
 * - `jd_satools_control.service_stop(name)`
 * - `jd_satools_control.service_status(name)`
 *
 * Environment Management (`jd_satools_env`)**
 * - `jd_satools_env.get(variable)`
 * - `jd_satools_env.set(variable, value)`
 * - `jd_satools_env.list()`
 * - `jd_satools_env.unset(variable)`
 */

