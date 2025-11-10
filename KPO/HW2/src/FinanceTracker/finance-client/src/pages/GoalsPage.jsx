import React from "react";
import { useMutation, useQuery } from "@tanstack/react-query";
import { api } from "../api/api";
import {
    Alert,
    Box,
    Button,
    Grid,
    MenuItem,
    Paper,
    Stack,
    TextField,
    Typography,
} from "@mui/material";
import { useCurrentUser } from "../state/UserContext";
import GoalCard from "../components/GoalCard";

export default function GoalsPage() {
    const { user } = useCurrentUser();
    const enabled = !!user?.id;

    const qGoals = useQuery({
        queryKey: ["goals", user?.id],
        queryFn: async () => (await api.get(`/goals/${user.id}`)).data,
        enabled,
    });

    const qAcc = useQuery({
        queryKey: ["accounts", user?.id],
        queryFn: async () => (await api.get(`/accounts/${user.id}`)).data,
        enabled,
    });

    const create = useMutation({
        mutationFn: (payload) => api.post("/goals", payload),
        onSuccess: () => qGoals.refetch(),
    });

    const topUp = useMutation({
        mutationFn: ({ goalId, fromId, amount }) =>
            api.post(`/goals/${goalId}/deposit`, {
                userId: user.id,
                fromId,
                amount,
            }),
        onSuccess: () => {
            qGoals.refetch();
            qAcc.refetch();
        },
    });

    if (!user)
        return (
            <Alert severity="info">
                Выберите пользователя во вкладке "Пользователи".
            </Alert>
        );

    const accountsById = Object.fromEntries(
        (qAcc.data ?? []).map((a) => [a.id, a])
    );
    const goalCards = (qGoals.data ?? []).map((g) => ({
        goal: g,
        balance: accountsById[g.accountId]?.balance ?? 0,
    }));

    const goalAccounts = (qAcc.data ?? []).filter((a) =>
        a.name.toLowerCase().startsWith("цель:")
    );
    const normalAccounts = (qAcc.data ?? []).filter(
        (a) => !a.name.toLowerCase().startsWith("цель:")
    );

    return (
        <Stack spacing={3}>
            <Typography variant="h5">🎯 Финансовые цели</Typography>

            {/* Создание новой цели */}
            <Paper sx={{ p: 2 }}>
                <NewGoalForm
                    onSubmit={(p) => create.mutate({ ...p, userId: user.id })}
                />
            </Paper>

            {/* Пополнение цели */}
            <Paper sx={{ p: 2 }}>
                <TopUpGoalForm
                    goals={qGoals.data ?? []}
                    goalAccounts={goalAccounts}
                    normalAccounts={normalAccounts}
                    onSubmit={(p) => topUp.mutate(p)}
                />
            </Paper>

            {/* Список целей */}
            <Grid container spacing={2}>
                {goalCards.map(({ goal, balance }) => (
                    <Grid item xs={12} md={6} lg={4} key={goal.id}>
                        <GoalCard goal={goal} accountBalance={balance} />
                    </Grid>
                ))}
            </Grid>
        </Stack>
    );
}

function NewGoalForm({ onSubmit }) {
    const [name, setName] = React.useState("");
    const [targetAmount, setTargetAmount] = React.useState("");
    const [deadline, setDeadline] = React.useState(
        new Date(Date.now() + 30 * 24 * 60 * 60 * 1000)
            .toISOString()
            .slice(0, 10)
    );

    return (
        <Box
            component="form"
            onSubmit={(e) => {
                e.preventDefault();
                if (name && +targetAmount > 0)
                    onSubmit({ name, targetAmount: +targetAmount, deadline });
                setName("");
                setTargetAmount("");
            }}
            sx={{
                display: "grid",
                gap: 2,
                gridTemplateColumns: { xs: "1fr", md: "1fr 1fr 1fr" },
            }}
        >
            <TextField
                label="Название цели"
                value={name}
                onChange={(e) => setName(e.target.value)}
            />
            <TextField
                type="number"
                label="Целевая сумма"
                value={targetAmount}
                onChange={(e) => setTargetAmount(e.target.value)}
            />
            <TextField
                type="date"
                label="Дедлайн"
                value={deadline}
                onChange={(e) => setDeadline(e.target.value)}
            />
            <Button type="submit" variant="contained">
                Создать
            </Button>
        </Box>
    );
}

function TopUpGoalForm({ goals, goalAccounts, normalAccounts, onSubmit }) {
    const [goalId, setGoalId] = React.useState("");
    const [fromId, setFromId] = React.useState("");
    const [amount, setAmount] = React.useState("");

    const selectedGoal = goals.find((g) => g.id === goalId);
    const goalAccount = goalAccounts.find((a) =>
        a.name.toLowerCase().includes(selectedGoal?.name?.toLowerCase() ?? "")
    );
    const goalAccountId = goalAccount?.id;

    const valid = goalId && fromId && fromId !== goalAccountId && +amount > 0;

    return (
        <Box
            component="form"
            onSubmit={(e) => {
                e.preventDefault();
                if (valid) onSubmit({ goalId, fromId, amount: +amount });
                setAmount("");
            }}
            sx={{
                display: "grid",
                gap: 2,
                gridTemplateColumns: { xs: "1fr", sm: "1fr 1fr 1fr" },
            }}
        >
            <TextField
                select
                label="Цель"
                value={goalId}
                onChange={(e) => setGoalId(e.target.value)}
            >
                {goals.map((g) => (
                    <MenuItem key={g.id} value={g.id}>
                        {g.name}
                    </MenuItem>
                ))}
            </TextField>

            <TextField
                select
                label="Счёт-источник"
                value={fromId}
                onChange={(e) => setFromId(e.target.value)}
            >
                {normalAccounts.map((a) => (
                    <MenuItem key={a.id} value={a.id}>
                        {a.name} — {a.balance} {a.currency}
                    </MenuItem>
                ))}
            </TextField>

            <TextField
                type="number"
                label="Сумма"
                value={amount}
                onChange={(e) => setAmount(e.target.value)}
            />
            <Button type="submit" variant="contained" disabled={!valid}>
                Пополнить
            </Button>
        </Box>
    );
}