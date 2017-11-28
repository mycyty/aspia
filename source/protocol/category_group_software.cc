//
// PROJECT:         Aspia Remote Desktop
// FILE:            protocol/category_group_software.cc
// LICENSE:         Mozilla Public License Version 2.0
// PROGRAMMERS:     Dmitry Chapyshev (dmitry@aspia.ru)
//

#include "base/process/process_enumerator.h"
#include "base/program_enumerator.h"
#include "base/service_enumerator.h"
#include "protocol/category_group_software.h"
#include "ui/system_info/output_proxy.h"
#include "ui/resource.h"

namespace aspia {

//
// CategoryPrograms
//

const char* CategoryPrograms::Name() const
{
    return "Programs";
}

Category::IconId CategoryPrograms::Icon() const
{
    return IDI_APPLICATIONS;
}

const char* CategoryPrograms::Guid() const
{
    return "606C70BE-0C6C-4CB6-90E6-D374760FC5EE";
}

void CategoryPrograms::Parse(std::shared_ptr<OutputProxy> output, const std::string& data)
{
    system_info::Programs message;

    if (!message.ParseFromString(data))
        return;

    Output::Table table(output, Name());

    {
        Output::TableHeader header(output);
        output->AddHeaderItem("Name", 200);
        output->AddHeaderItem("Version", 100);
        output->AddHeaderItem("Publisher", 100);
        output->AddHeaderItem("Install Date", 80);
        output->AddHeaderItem("Install Location", 150);
    }

    for (int index = 0; index < message.item_size(); ++index)
    {
        const system_info::Programs::Item& item = message.item(index);

        Output::Row row(output, Icon());

        output->AddValue(item.name());
        output->AddValue(item.version());
        output->AddValue(item.publisher());
        output->AddValue(item.install_date());
        output->AddValue(item.install_location());
    }
}

std::string CategoryPrograms::Serialize()
{
    system_info::Programs message;

    for (ProgramEnumerator enumerator; !enumerator.IsAtEnd(); enumerator.Advance())
    {
        system_info::Programs::Item* item = message.add_item();

        item->set_name(enumerator.GetName());
        item->set_version(enumerator.GetVersion());
        item->set_publisher(enumerator.GetPublisher());
        item->set_install_date(enumerator.GetInstallDate());
        item->set_install_location(enumerator.GetInstallLocation());
    }

    return message.SerializeAsString();
}

//
// CategoryUpdates
//

const char* CategoryUpdates::Name() const
{
    return "Updates";
}

Category::IconId CategoryUpdates::Icon() const
{
    return IDI_APPLICATIONS;
}

const char* CategoryUpdates::Guid() const
{
    return "3E160E27-BE2E-45DB-8292-C3786C9533AB";
}

void CategoryUpdates::Parse(std::shared_ptr<OutputProxy> output, const std::string& data)
{
    UNUSED_PARAMETER(output);
    UNUSED_PARAMETER(data);
    // TODO
}

std::string CategoryUpdates::Serialize()
{
    // TODO
    return std::string();
}

//
// CategoryServices
//

const char* CategoryServices::Name() const
{
    return "Services";
}

Category::IconId CategoryServices::Icon() const
{
    return IDI_GEAR;
}

const char* CategoryServices::Guid() const
{
    return "BE3143AB-67C3-4EFE-97F5-FA0C84F338C3";
}

void CategoryServices::Parse(std::shared_ptr<OutputProxy> output, const std::string& data)
{
    system_info::Services message;

    if (!message.ParseFromString(data))
        return;

    Output::Table table(output, Name());

    {
        Output::TableHeader header(output);
        output->AddHeaderItem("Display Name", 200);
        output->AddHeaderItem("Name", 200);
        output->AddHeaderItem("Description", 200);
        output->AddHeaderItem("Status", 200);
        output->AddHeaderItem("Startup Type", 200);
        output->AddHeaderItem("Account", 200);
        output->AddHeaderItem("Executable File", 200);
    }

    for (int index = 0; index < message.item_size(); ++index)
    {
        const system_info::Services::Item& item = message.item(index);

        Output::Row row(output, Icon());

        output->AddValue(item.display_name());
        output->AddValue(item.name());
        output->AddValue(item.description());
        output->AddValue(StatusToString(item.status()));
        output->AddValue(StartupTypeToString(item.startup_type()));
        output->AddValue(item.start_name());
        output->AddValue(item.binary_path());
    }
}

std::string CategoryServices::Serialize()
{
    system_info::Services message;

    for (ServiceEnumerator enumerator(ServiceEnumerator::Type::SERVICES);
         !enumerator.IsAtEnd();
         enumerator.Advance())
    {
        system_info::Services::Item* item = message.add_item();

        item->set_name(enumerator.GetName());
        item->set_display_name(enumerator.GetDisplayName());
        item->set_description(enumerator.GetDescription());

        switch (enumerator.GetStatus())
        {
            case ServiceEnumerator::Status::CONTINUE_PENDING:
                item->set_status(system_info::Services::Item::STATUS_CONTINUE_PENDING);
                break;

            case ServiceEnumerator::Status::PAUSE_PENDING:
                item->set_status(system_info::Services::Item::STATUS_PAUSE_PENDING);
                break;

            case ServiceEnumerator::Status::PAUSED:
                item->set_status(system_info::Services::Item::STATUS_PAUSED);
                break;

            case ServiceEnumerator::Status::RUNNING:
                item->set_status(system_info::Services::Item::STATUS_RUNNING);
                break;

            case ServiceEnumerator::Status::START_PENDING:
                item->set_status(system_info::Services::Item::STATUS_START_PENDING);
                break;

            case ServiceEnumerator::Status::STOP_PENDING:
                item->set_status(system_info::Services::Item::STATUS_STOP_PENDING);
                break;

            case ServiceEnumerator::Status::STOPPED:
                item->set_status(system_info::Services::Item::STATUS_STOPPED);
                break;

            default:
                item->set_status(system_info::Services::Item::STATUS_UNKNOWN);
                break;
        }

        switch (enumerator.GetStartupType())
        {
            case ServiceEnumerator::StartupType::AUTO_START:
                item->set_startup_type(system_info::Services::Item::STARTUP_TYPE_AUTO_START);
                break;

            case ServiceEnumerator::StartupType::DEMAND_START:
                item->set_startup_type(system_info::Services::Item::STARTUP_TYPE_DEMAND_START);
                break;

            case ServiceEnumerator::StartupType::DISABLED:
                item->set_startup_type(system_info::Services::Item::STARTUP_TYPE_DISABLED);
                break;

            case ServiceEnumerator::StartupType::BOOT_START:
                item->set_startup_type(system_info::Services::Item::STARTUP_TYPE_BOOT_START);
                break;

            case ServiceEnumerator::StartupType::SYSTEM_START:
                item->set_startup_type(system_info::Services::Item::STARTUP_TYPE_SYSTEM_START);
                break;

            default:
                item->set_startup_type(system_info::Services::Item::STARTUP_TYPE_UNKNOWN);
                break;
        }

        item->set_binary_path(enumerator.GetBinaryPath());
        item->set_start_name(enumerator.GetStartName());
    }

    return message.SerializeAsString();
}

    // static
const char* CategoryServices::StatusToString(system_info::Services::Item::Status status)
{
    switch (status)
    {
        case system_info::Services::Item::STATUS_CONTINUE_PENDING:
            return "Continue Pending";

        case system_info::Services::Item::STATUS_PAUSE_PENDING:
            return "Pause Pending";

        case system_info::Services::Item::STATUS_PAUSED:
            return "Paused";

        case system_info::Services::Item::STATUS_RUNNING:
            return "Running";

        case system_info::Services::Item::STATUS_START_PENDING:
            return "Start Pending";

        case system_info::Services::Item::STATUS_STOP_PENDING:
            return "Stop Pending";

        case system_info::Services::Item::STATUS_STOPPED:
            return "Stopped";

        default:
            return "Unknown";
    }
}

    // static
const char* CategoryServices::StartupTypeToString(
    system_info::Services::Item::StartupType startup_type)
{
    switch (startup_type)
    {
        case system_info::Services::Item::STARTUP_TYPE_AUTO_START:
            return "Auto Start";

        case system_info::Services::Item::STARTUP_TYPE_DEMAND_START:
            return "Demand Start";

        case system_info::Services::Item::STARTUP_TYPE_DISABLED:
            return "Disabled";

        case system_info::Services::Item::STARTUP_TYPE_BOOT_START:
            return "Boot Start";

        case system_info::Services::Item::STARTUP_TYPE_SYSTEM_START:
            return "System Start";

        default:
            return "Unknown";
    }
}

//
// CategoryDrivers
//

const char* CategoryDrivers::Name() const
{
    return "Drivers";
}

Category::IconId CategoryDrivers::Icon() const
{
    return IDI_PCI;
}

const char* CategoryDrivers::Guid() const
{
    return "8278DA10-227F-4484-9D5D-9A66C294CA82";
}

void CategoryDrivers::Parse(std::shared_ptr<OutputProxy> output, const std::string& data)
{
    system_info::Services message;

    if (!message.ParseFromString(data))
        return;

    Output::Table table(output, Name());

    {
        Output::TableHeader header(output);
        output->AddHeaderItem("Display Name", 200);
        output->AddHeaderItem("Name", 200);
        output->AddHeaderItem("Description", 200);
        output->AddHeaderItem("Status", 200);
        output->AddHeaderItem("Startup Type", 200);
        output->AddHeaderItem("Executable File", 200);
    }

    for (int index = 0; index < message.item_size(); ++index)
    {
        const system_info::Services::Item& item = message.item(index);

        Output::Row row(output, Icon());

        output->AddValue(item.display_name());
        output->AddValue(item.name());
        output->AddValue(item.description());
        output->AddValue(CategoryServices::StatusToString(item.status()));
        output->AddValue(CategoryServices::StartupTypeToString(item.startup_type()));
        output->AddValue(item.binary_path());
    }
}

std::string CategoryDrivers::Serialize()
{
    system_info::Services message;

    for (ServiceEnumerator enumerator(ServiceEnumerator::Type::DRIVERS);
         !enumerator.IsAtEnd();
         enumerator.Advance())
    {
        system_info::Services::Item* item = message.add_item();

        item->set_name(enumerator.GetName());
        item->set_display_name(enumerator.GetDisplayName());
        item->set_description(enumerator.GetDescription());

        switch (enumerator.GetStatus())
        {
            case ServiceEnumerator::Status::CONTINUE_PENDING:
                item->set_status(system_info::Services::Item::STATUS_CONTINUE_PENDING);
                break;

            case ServiceEnumerator::Status::PAUSE_PENDING:
                item->set_status(system_info::Services::Item::STATUS_PAUSE_PENDING);
                break;

            case ServiceEnumerator::Status::PAUSED:
                item->set_status(system_info::Services::Item::STATUS_PAUSED);
                break;

            case ServiceEnumerator::Status::RUNNING:
                item->set_status(system_info::Services::Item::STATUS_RUNNING);
                break;

            case ServiceEnumerator::Status::START_PENDING:
                item->set_status(system_info::Services::Item::STATUS_START_PENDING);
                break;

            case ServiceEnumerator::Status::STOP_PENDING:
                item->set_status(system_info::Services::Item::STATUS_STOP_PENDING);
                break;

            case ServiceEnumerator::Status::STOPPED:
                item->set_status(system_info::Services::Item::STATUS_STOPPED);
                break;

            default:
                item->set_status(system_info::Services::Item::STATUS_UNKNOWN);
                break;
        }

        switch (enumerator.GetStartupType())
        {
            case ServiceEnumerator::StartupType::AUTO_START:
                item->set_startup_type(system_info::Services::Item::STARTUP_TYPE_AUTO_START);
                break;

            case ServiceEnumerator::StartupType::DEMAND_START:
                item->set_startup_type(system_info::Services::Item::STARTUP_TYPE_DEMAND_START);
                break;

            case ServiceEnumerator::StartupType::DISABLED:
                item->set_startup_type(system_info::Services::Item::STARTUP_TYPE_DISABLED);
                break;

            case ServiceEnumerator::StartupType::BOOT_START:
                item->set_startup_type(system_info::Services::Item::STARTUP_TYPE_BOOT_START);
                break;

            case ServiceEnumerator::StartupType::SYSTEM_START:
                item->set_startup_type(system_info::Services::Item::STARTUP_TYPE_SYSTEM_START);
                break;

            default:
                item->set_startup_type(system_info::Services::Item::STARTUP_TYPE_UNKNOWN);
                break;
        }

        item->set_binary_path(enumerator.GetBinaryPath());
        item->set_start_name(enumerator.GetStartName());
    }

    return message.SerializeAsString();
}

//
// CategoryProcesses
//

const char* CategoryProcesses::Name() const
{
    return "Processes";
}

Category::IconId CategoryProcesses::Icon() const
{
    return IDI_SYSTEM_MONITOR;
}

const char* CategoryProcesses::Guid() const
{
    return "14BB101B-EE61-49E6-B5B9-874C4DBEA03C";
}

void CategoryProcesses::Parse(std::shared_ptr<OutputProxy> output, const std::string& data)
{
    system_info::Processes message;

    if (!message.ParseFromString(data))
        return;

    Output::Table table(output, Name());

    {
        Output::TableHeader header(output);
        output->AddHeaderItem("Process Name", 150);
        output->AddHeaderItem("File Path", 200);
        output->AddHeaderItem("Used Memory", 80);
        output->AddHeaderItem("Used Swap", 80);
        output->AddHeaderItem("Description", 150);
    }

    for (int index = 0; index < message.item_size(); ++index)
    {
        const system_info::Processes::Item& item = message.item(index);

        Output::Row row(output, Icon());

        output->AddValue(item.process_name());
        output->AddValue(item.file_path());

        if (item.used_memory() != 0)
        {
            output->AddValue(std::to_string(item.used_memory() / 1024), "kB");
        }
        else
        {
            output->AddValue(std::string());
        }

        if (item.used_swap() != 0)
        {
            output->AddValue(std::to_string(item.used_swap() / 1024), "kB");
        }
        else
        {
            output->AddValue(std::string());
        }

        output->AddValue(item.description());
    }
}

std::string CategoryProcesses::Serialize()
{
    system_info::Processes message;

    for (ProcessEnumerator enumerator; !enumerator.IsAtEnd(); enumerator.Advance())
    {
        system_info::Processes::Item* item = message.add_item();

        item->set_process_name(enumerator.GetProcessName());
        item->set_file_path(enumerator.GetFilePath());
        item->set_used_memory(enumerator.GetUsedMemory());
        item->set_used_swap(enumerator.GetUsedSwap());
        item->set_description(enumerator.GetFileDescription());
    }

    return message.SerializeAsString();
}

//
// CategoryLicenses
//

const char* CategoryLicenses::Name() const
{
    return "Licenses";
}

Category::IconId CategoryLicenses::Icon() const
{
    return IDI_LICENSE_KEY;
}

const char* CategoryLicenses::Guid() const
{
    return "6BD88575-9D23-44BC-8A49-64D94CC3EE48";
}

void CategoryLicenses::Parse(std::shared_ptr<OutputProxy> output, const std::string& data)
{
    UNUSED_PARAMETER(output);
    UNUSED_PARAMETER(data);
    // TODO
}

std::string CategoryLicenses::Serialize()
{
    // TODO
    return std::string();
}

//
// CategoryGroupSoftware
//

const char* CategoryGroupSoftware::Name() const
{
    return "Software";
}

Category::IconId CategoryGroupSoftware::Icon() const
{
    return IDI_SOFTWARE;
}

} // namespace aspia