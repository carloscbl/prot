// generated by /usr/local/bin/sqlpp11-ddl2cpp ../db_schemas/test_prot.sql test_prot test_prot
#ifndef TEST_PROT_TEST_PROT_H
#define TEST_PROT_TEST_PROT_H

#include <sqlpp11/table.h>
#include <sqlpp11/data_types.h>
#include <sqlpp11/char_sequence.h>

namespace test_prot
{
  namespace Schedulers_
  {
    struct Idscheduler
    {
      struct _alias_t
      {
        static constexpr const char _literal[] =  "idscheduler";
        using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
        template<typename T>
        struct _member_t
          {
            T idscheduler;
            T& operator()() { return idscheduler; }
            const T& operator()() const { return idscheduler; }
          };
      };
      using _traits = sqlpp::make_traits<sqlpp::integer, sqlpp::tag::require_insert>;
    };
  } // namespace Schedulers_

  struct Schedulers: sqlpp::table_t<Schedulers,
               Schedulers_::Idscheduler>
  {
    struct _alias_t
    {
      static constexpr const char _literal[] =  "schedulers";
      using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
      template<typename T>
      struct _member_t
      {
        T schedulers;
        T& operator()() { return schedulers; }
        const T& operator()() const { return schedulers; }
      };
    };
  };
  namespace Taskers_
  {
    struct Idtasker
    {
      struct _alias_t
      {
        static constexpr const char _literal[] =  "idtasker";
        using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
        template<typename T>
        struct _member_t
          {
            T idtasker;
            T& operator()() { return idtasker; }
            const T& operator()() const { return idtasker; }
          };
      };
      using _traits = sqlpp::make_traits<sqlpp::integer, sqlpp::tag::require_insert>;
    };
    struct Users
    {
      struct _alias_t
      {
        static constexpr const char _literal[] =  "users";
        using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
        template<typename T>
        struct _member_t
          {
            T users;
            T& operator()() { return users; }
            const T& operator()() const { return users; }
          };
      };
      using _traits = sqlpp::make_traits<sqlpp::varchar, sqlpp::tag::require_insert>;
    };
  } // namespace Taskers_

  struct Taskers: sqlpp::table_t<Taskers,
               Taskers_::Idtasker,
               Taskers_::Users>
  {
    struct _alias_t
    {
      static constexpr const char _literal[] =  "taskers";
      using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
      template<typename T>
      struct _member_t
      {
        T taskers;
        T& operator()() { return taskers; }
        const T& operator()() const { return taskers; }
      };
    };
  };
  namespace Tasks_
  {
    struct Id
    {
      struct _alias_t
      {
        static constexpr const char _literal[] =  "id";
        using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
        template<typename T>
        struct _member_t
          {
            T id;
            T& operator()() { return id; }
            const T& operator()() const { return id; }
          };
      };
      using _traits = sqlpp::make_traits<sqlpp::integer, sqlpp::tag::require_insert>;
    };
    struct Name
    {
      struct _alias_t
      {
        static constexpr const char _literal[] =  "name";
        using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
        template<typename T>
        struct _member_t
          {
            T name;
            T& operator()() { return name; }
            const T& operator()() const { return name; }
          };
      };
      using _traits = sqlpp::make_traits<sqlpp::varchar, sqlpp::tag::can_be_null>;
    };
    struct Description
    {
      struct _alias_t
      {
        static constexpr const char _literal[] =  "description";
        using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
        template<typename T>
        struct _member_t
          {
            T description;
            T& operator()() { return description; }
            const T& operator()() const { return description; }
          };
      };
      using _traits = sqlpp::make_traits<sqlpp::varchar, sqlpp::tag::can_be_null>;
    };
  } // namespace Tasks_

  struct Tasks: sqlpp::table_t<Tasks,
               Tasks_::Id,
               Tasks_::Name,
               Tasks_::Description>
  {
    struct _alias_t
    {
      static constexpr const char _literal[] =  "tasks";
      using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
      template<typename T>
      struct _member_t
      {
        T tasks;
        T& operator()() { return tasks; }
        const T& operator()() const { return tasks; }
      };
    };
  };
  namespace TasksSchedulers_
  {
    struct Idtask
    {
      struct _alias_t
      {
        static constexpr const char _literal[] =  "idtask";
        using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
        template<typename T>
        struct _member_t
          {
            T idtask;
            T& operator()() { return idtask; }
            const T& operator()() const { return idtask; }
          };
      };
      using _traits = sqlpp::make_traits<sqlpp::integer, sqlpp::tag::require_insert>;
    };
    struct Idscheduler
    {
      struct _alias_t
      {
        static constexpr const char _literal[] =  "idscheduler";
        using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
        template<typename T>
        struct _member_t
          {
            T idscheduler;
            T& operator()() { return idscheduler; }
            const T& operator()() const { return idscheduler; }
          };
      };
      using _traits = sqlpp::make_traits<sqlpp::integer, sqlpp::tag::require_insert>;
    };
  } // namespace TasksSchedulers_

  struct TasksSchedulers: sqlpp::table_t<TasksSchedulers,
               TasksSchedulers_::Idtask,
               TasksSchedulers_::Idscheduler>
  {
    struct _alias_t
    {
      static constexpr const char _literal[] =  "tasks_schedulers";
      using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
      template<typename T>
      struct _member_t
      {
        T tasksSchedulers;
        T& operator()() { return tasksSchedulers; }
        const T& operator()() const { return tasksSchedulers; }
      };
    };
  };
  namespace TasksTaskers_
  {
    struct Idtask
    {
      struct _alias_t
      {
        static constexpr const char _literal[] =  "idtask";
        using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
        template<typename T>
        struct _member_t
          {
            T idtask;
            T& operator()() { return idtask; }
            const T& operator()() const { return idtask; }
          };
      };
      using _traits = sqlpp::make_traits<sqlpp::integer, sqlpp::tag::require_insert>;
    };
    struct Idtasker
    {
      struct _alias_t
      {
        static constexpr const char _literal[] =  "idtasker";
        using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
        template<typename T>
        struct _member_t
          {
            T idtasker;
            T& operator()() { return idtasker; }
            const T& operator()() const { return idtasker; }
          };
      };
      using _traits = sqlpp::make_traits<sqlpp::integer, sqlpp::tag::require_insert>;
    };
  } // namespace TasksTaskers_

  struct TasksTaskers: sqlpp::table_t<TasksTaskers,
               TasksTaskers_::Idtask,
               TasksTaskers_::Idtasker>
  {
    struct _alias_t
    {
      static constexpr const char _literal[] =  "tasks_taskers";
      using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
      template<typename T>
      struct _member_t
      {
        T tasksTaskers;
        T& operator()() { return tasksTaskers; }
        const T& operator()() const { return tasksTaskers; }
      };
    };
  };
  namespace Users_
  {
    struct Name
    {
      struct _alias_t
      {
        static constexpr const char _literal[] =  "name";
        using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
        template<typename T>
        struct _member_t
          {
            T name;
            T& operator()() { return name; }
            const T& operator()() const { return name; }
          };
      };
      using _traits = sqlpp::make_traits<sqlpp::varchar, sqlpp::tag::require_insert>;
    };
  } // namespace Users_

  struct Users: sqlpp::table_t<Users,
               Users_::Name>
  {
    struct _alias_t
    {
      static constexpr const char _literal[] =  "users";
      using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
      template<typename T>
      struct _member_t
      {
        T users;
        T& operator()() { return users; }
        const T& operator()() const { return users; }
      };
    };
  };
} // namespace test_prot
#endif